#include "PlayerActor.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AudioManager.h"

#include "PlayerActorStateBase.h"
#include "PlayerActorStateRun.h"
#include "PlayerActorStateIdle.h"
#include "PlayerActorStateJump.h"
#include "PlayerActorStateJumpLoop.h"
#include "PlayerActorStateJumpEnd.h"
#include "PlayerActorStateLightAttack1.h"
#include "PlayerActorStateLightAttack2.h"
#include "PlayerActorStateLightAttack3.h"
#include "PlayerActorStateHIt.h"

#include "EnemyBehaviorComponent.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyHit.h"
#include "EnemyShot.h"

#include <iostream>

const float cAnimationSpeed = 0.5f;

PlayerActor::PlayerActor()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mIsJump(false)
	, mOnGroundChk(true)
	, mPHitPoint(10)		//体力
{

	//メッシュのロード
	Mesh* mesh = RENDERER->GetMesh("Assets/Player/Player_Idle.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// スケルトン
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Player/Player_Idle.gpskel"));
	
	// アニメーションの取得 & アニメーション配列にセット
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	//アイドル
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)]      = RENDERER->GetAnimation("Assets/Player/Player_Idle_Anim.gpanim", true);
	//走る
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)]       = RENDERER->GetAnimation("Assets/Player/Player_Running.gpanim", true);
	//ジャンプスタート
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPSTART)] = RENDERER->GetAnimation("Assets/Player/Jumping_Up.gpanim", false);
	//ジャンプループ
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPLOOP)]  = RENDERER->GetAnimation("Assets/Player/Player_Falling.gpanim", false);
	//ジャンプ終わり
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPEND)]	 = RENDERER->GetAnimation("Assets/Player/Jumping_Down.gpanim", false);
	//弱攻撃1
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_LIGHTATTACK1)] = RENDERER->GetAnimation("Assets/Player/Player_Attack1.gpanim", false);
	//弱攻撃2
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_LIGHTATTACK2)] = RENDERER->GetAnimation("Assets/Player/Player_Attack2.gpanim", false);
	//弱攻撃3
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_LIGHTATTACK3)] = RENDERER->GetAnimation("Assets/Player/Player_Attack3.gpanim", false);
	//ダメージ
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_HIT)] = RENDERER->GetAnimation("Assets/Player/Player_Hit.gpanim", false);


	// アイドル状態アニメーションをセット
	mMeshComp->PlayAnimation( mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)], cAnimationSpeed);

	// アクターステートプールの初期化
	mStatePools.push_back(new PlayerActorStateIdle);          // mStatePool[PLAYER_STATE_IDLE]
	mStatePools.push_back(new PlayerActorStateRun);           // mStatepool[PLAYER_STATE_RUN]
	mStatePools.push_back(new PlayerActorStateJump);          // mStatepool[PLAYER_STATE_JUMPSTART];  
	mStatePools.push_back(new PlayerActorStateJumpLoop);      // mStatepool[PLAYER_STATE_JUMPLOOP];
	mStatePools.push_back(new PlayerActorStateJumpEnd);       // mStatepool[PLAYER_STATE_JUMPEND];
	mStatePools.push_back(new PlayerActorStateLightAttack1);  // mStatepool[PLAYER_STATE_LightAttack1];
	mStatePools.push_back(new PlayerActorStateLightAttack2);  // mStatepool[PLAYER_STATE_LightAttack2];
	mStatePools.push_back(new PlayerActorStateLightAttack3);  // mStatepool[PLAYER_STATE_LightAttack3];
	mStatePools.push_back(new PlayerActorStateHIt);			  // mStatepool[PLAYER_STATE_HIT];

	// あたり判定セット
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this,EnumPhysicsType::EnumPlayer);
	playerBox.mMin.x *= 0.8f;
	playerBox.mMin.y *= 0.8f;
	playerBox.mMax.x *= 0.8f;
	playerBox.mMax.y *= 0.8f;
	mHitBox->SetObjectBox(playerBox);

	// プレーヤーの足元を調べるボックスを作成　ボックス高1/4, ボックス上面が原点に来るようにする
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //ジャンプ時に引っかからない高さ
	groundBox.mMax.z *= 0.0f;
	mHitGroundBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitGroundBox->SetObjectBox(groundBox);

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitHeadBox->SetObjectBox(headBox);
	
	printf("PlayerActor作成 id:[%5d] this : (0x%p)\n", mID, this);
	mJumpVec = Vector3(0, 0, 0);

	//プレイヤーの位置を取得
	GAMEINSTANCE.SetPlayerActor(this);
}

PlayerActor::~PlayerActor()
{
	//登録ステートをすべて解放
	for (auto ps : mStatePools)
	{
		delete ps;
	}
	mAnimTypes.clear(); //アニメーション本体の消去はレンダラー側で行われる
	printf("PlayerActor破棄 id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	// 前のシーンから床がなかったら
	if (!mIsJump && !mOnGroundChk)
	{
		mIsJump = true;
		mNextState = PlayerState::PLAYER_STATE_JUMPLOOP;
	}
	mOnGroundChk = false;

	// ステート外部からステート変更があったか？
	if (mNowState != mNextState)
	{
		//今のステートのExitに入り
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		//次のステートのEnterに進む
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// ステート内部からステート変更あったか？
	if (mNowState != mNextState)
	{
		//今のステートのExitに入り
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		//次のステートのEnterに進む
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	//武器の当たり判定の更新処理

}


// 背景AABBとのヒットめり込み解消 ( 当たった際にPhysicsWorldから呼ばれる ）
void PlayerActor::FixCollision(BoxCollider *hitPlayerBox, BoxCollider * hitBox)
{

	Vector3 fix;
	// ジャンプ終了か
	if (hitPlayerBox == mHitGroundBox && mJumpVec.z <= 0.0f)
	{
		// 地面判定用ボックスが地面と当たっていた(接地チェック）
		mOnGroundChk = true;

		// ジャンプ中なら解除
		if (mIsJump)
		{
			mOnGroundChk = true;
			mIsJump = false;
			mJumpVec = Vector3(0, 0, -1.0f);
			mPosition += mJumpVec;
			mNextState = PlayerState::PLAYER_STATE_JUMPEND;
			return;
		}
	}

	//壁とぶつかったとき
	AABB bgBox = hitBox->GetWorldBox();
	AABB playerBox = mHitBox->GetWorldBox();

	// めり込みを修正
	calcCollisionFixVec(playerBox, bgBox, fix);

	// 補正ベクトル分戻す
	mPosition += fix;

	// ジャンプアクション中に頭が壁に当たったとき
	if (hitPlayerBox == mHitHeadBox && mIsJump)
	{
		//ジャンプ上向きに行く方向を逆転させる
		mJumpVec = Vector3(0, 0, -1.0f);
		mPosition += mJumpVec; // もう頭上に当たらない様に離す
	}

	// 位置が変わったのでボックス再計算
	mHitBox->OnUpdateWorldTransform();

}

//敵との当たり判定
void PlayerActor::FixEnemyCollision(BoxCollider* hitPlayerBox, BoxCollider* enemyBox)
{
	if (hitPlayerBox != mHitBox)
	{
		return;
	}
	Vector3 fix;

	//壁とぶつかったとき
	AABB EBox = enemyBox->GetWorldBox();
	AABB playerBox = hitPlayerBox->GetWorldBox();

	//めり込み修正
	calcCollisionFixVec(playerBox,EBox, fix);

	//補正ベクトル分戻す
	mPosition += fix;

	//位置が変わったのでボックスを再計算
	hitPlayerBox->OnUpdateWorldTransform();
	mRecomputeWorldTransform = true;
}

SkeletalMeshComponent* PlayerActor::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerActor::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void PlayerActor::SetAttackHitBox()
{
	//プレイヤー攻撃用ボックス設置
	AABB playerAttackBox;

	mAttackBox = new BoxCollider(this, EnumPhysicsType::EnumPlayerAttack);

	playerAttackBox.mMin.x = 10;
	playerAttackBox.mMin.y = -50;
	playerAttackBox.mMin.z = 100;

	playerAttackBox.mMax.x = playerAttackBox.mMin.x + 150.0f;
	playerAttackBox.mMax.y = playerAttackBox.mMin.y + 150.0f;
	playerAttackBox.mMax.z = playerAttackBox.mMin.z + 150.0f;
	mAttackBox->SetObjectBox(playerAttackBox);
	mAttackBox->OnUpdateWorldTransform();
}

//攻撃ボックスの消去
void PlayerActor::RemoveAttackHitBox()
{
	//プレイヤーの攻撃ボックスが敵に当たったら
	if (mAttackBox)
	{
		//攻撃のボックスを消す
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

//敵との当たり判定
void PlayerActor::HitEnemy(class BoxCollider* playerBox, BoxCollider* enemyBox)
{
	//敵に攻撃が当たったとき
	if (playerBox->GetType() == EnumPhysicsType::EnumPlayerAttack &&
		enemyBox->GetType() == EnumPhysicsType::EnumEnemy)
	{
		//当たったときにすぐ消す
		RemoveAttackHitBox();
	}
}

//敵との当たり判定(ダメージ)
void PlayerActor::HitBullet(BoxCollider* hitPlayerBox, BoxCollider* mEnemyShotBox)
{
	//敵の攻撃が当たったとき
	if (mHitBox == hitPlayerBox &&
		mEnemyShotBox->GetType() == EnumPhysicsType::EnumEnemyShot)
	{
		printf("敵の攻撃が当たってるよ\n");

		mPHitPoint -= 1;

		if (mNowState != PlayerState::PLAYER_STATE_HIT)
		{
			mNextState = PlayerState::PLAYER_STATE_HIT;
		}
	}

	//空中で当たったときに浮いてしまうのを防ぐ
	if (mIsJump)
	{
		mOnGroundChk = true;
		mIsJump = false;
		mJumpVec = Vector3(0, 0, -1.0f);
		mPosition += mJumpVec;
		return;
	}
}

//敵の弾とプレイヤーの攻撃ボックスとの当たり判定
void PlayerActor::PlayerABandEnemyBB(BoxCollider* mPlayerAttackBox, BoxCollider* mEnemyShotBox)
{
}