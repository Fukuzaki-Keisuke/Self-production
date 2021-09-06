#include "BlackKnightActor.h"

#include "game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Input.h"
#include "GameClearScene.h"

#include "EnemyBehaviorComponent.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyHit.h"
#include "EnemyDeath.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemyShot.h"

#include <iostream>

BlackKnightActor::BlackKnightActor()
{
	// 初期パラメータ設定
	mWalkSpeed  = 100.0f;
	mRunSpeed   = 200.0f;
	mTurnSpeed  = Math::Pi;
	mHitPoint   = 2;

	//地面との設置判定
	mIsOnGround = true;

	// モデル読み込み
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/BlackKnightSK.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Enemy/BlackKnightSK.gpskel"));

	// アニメーション読み込み
	mAnimations.emplace(EnemyStateEnum::Walk,       RENDERER->GetAnimation("Assets/Enemy/BlackKnight_WalkFWDAnim.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::LookAround, RENDERER->GetAnimation("Assets/Enemy/BlackKnight_SenseSomethingRPTAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::GetDamage,  RENDERER->GetAnimation("Assets/Enemy/BlackKnight_GetHitAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Die,	    RENDERER->GetAnimation("Assets/Enemy/BlackKnight_DieAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Chase,		RENDERER->GetAnimation("Assets/Enemy/BlackKnight_WalkFWDAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Attack1,    RENDERER->GetAnimation("Assets/Enemy/BlackKnight_Attack01Anim.gpanim", false));

	// EnemyBehaviorComponent に ふるまいを追加
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemyPatrol(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyLookAround(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyHit(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyAttack(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Walk);

	// 敵キャラの当たり判定を追加
	AABB enemyBox = mesh->GetCollisionBox();
	enemyBox.mMin.x *= 0.5f;
	enemyBox.mMax.x *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(enemyBox);
	mHitBox->SetArrowRotate(false);

	//エネミーの近接攻撃用ボックス
	AABB enemyForward;
	enemyForward.mMin.x = enemyBox.mMax.x;
	enemyForward.mMin.y = enemyBox.mMin.y;
	enemyForward.mMin.z = enemyBox.mMin.z + 100;
	enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;
	SetTriggerBox(EnemyTriggerEnum::ForwardBox, enemyForward);
}	

BlackKnightActor::~BlackKnightActor()
{
	std::cout << "BlackKnight破棄" << mID << this << std::endl;
}

void BlackKnightActor::UpdateActor(float deltaTime)
{

	// BlackKnightの行動はEnemyBehaviorComponentに一任

	if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	{
		std::cout << "ForwardBoxHit!!" << std::endl;
	}

	//体力が0になったらボックスも消す
	if (mHitPoint <= 0)
	{
		mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::Die);
		delete mHitBox;

		//エネミーを消す
		SetState(EDead);

		//倒したらクリア
		//残機を1減らす
		//mEnemyCount--;
	}

	//自爆ボタン
	//キーZ
	/*if (INPUT_INSTANCE.GetInput(KEY_A))
	{
		mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::Die);
		mRemaining--;
	}*/

}

void BlackKnightActor::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	// 当たり判定で帰ってきた結果がmHitBox、背景との衝突だった場合
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumBG)
	{
		//壁とエネミーにぶつかったとき
		AABB bgBox = hitOtherBox->GetWorldBox();
		AABB enemyBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		// めり込みを修正
		calcCollisionFixVec(enemyBox, bgBox, fixVec);
		mPosition += fixVec;
		//位置が変わったのでボックスを再計算
		mHitBox->OnUpdateWorldTransform();
	}

	//プレイヤーの攻撃がヒット
	if(mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayerAttack)
	{

		if (mNowState != EnemyStateEnum::GetDamage && mNowState != EnemyStateEnum::Die)
		{

			// 体力の減算
			mHitPoint -= 1;
			SetHitPoint(mHitPoint);
			printf("攻撃があたったよ\n");
			mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::GetDamage);
		}
	}
}

bool BlackKnightActor::IsFrontHit()
{
	return mFrontTriggerBox->IsTrigerHit();
}
