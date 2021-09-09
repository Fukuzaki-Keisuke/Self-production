#include "NearEnemy.h"

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
#include "NearEnemyIdle.h"
#include "NearEnemyLookAround.h"
#include "NearEnemyWalk.h"
#include "NearEnemyChase.h"
#include "NearEnemyAttack.h"
#include "NearEnemyAttack2.h"
#include "NearEnemyHit.h"
#include "NearEnemyDie.h"

#include "EnemyShot.h"

#include <iostream>

NearEnemy::NearEnemy()
{
	//初期パラメータ
	mWalkSpeed = 100.0f;
	mRunSpeed = 200.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 5;

	//地面との設置判定
	mIsOnGround = true;

	//モデルの読み込み
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/NearEnemy/NearEnemy_Idle.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Enemy/NearEnemy/NearEnemy_Idle.gpskel"));

	//アニメーションの読み込み
	//最後のtrueはループするかしないか
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Idle_Anim.gpanim",false));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Walk1.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Chase, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Walk2.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::LookAround, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_LookAround.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Attack1.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Attack2, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Attack2.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::GetDamage, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Hit.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Die.gpanim", false));

	// EnemyBehaviorComponentにふるまいを追加
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new NearEnemyIdle(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyWalk(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyChase(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyLookAround(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyAttack(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyAttack2(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyHit(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyDie(mEnemyBehaviorComponent));

	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Walk);

	//敵キャラの当たり判定を追加
	AABB enemyBox = mesh->GetCollisionBox();
	enemyBox.mMin.x *= 1.5f;
	enemyBox.mMax.x *= 1.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(enemyBox);
	mHitBox->SetArrowRotate(false);

	////エネミーの近接攻撃ボックス
	//AABB enemyForward;
	//enemyForward.mMin.x = enemyBox.mMax.x;
	//enemyForward.mMin.y = enemyBox.mMin.y;
	//enemyForward.mMin.z = enemyBox.mMin.z + 100;
	//enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	//enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	//enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;
	//SetTriggerBox(EnemyTriggerEnum::ForwardBox, enemyForward);

}

NearEnemy::~NearEnemy()
{
	std::cout << "NearEnemy破棄" << mID << this << std::endl;
}

void NearEnemy::UpdateActor(float deltaTime)
{
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

	//即死ボタン
	//キーZ
	/*if (INPUT_INSTANCE.GetInput(KEY_A))
	{
		mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::Die);
	}*/
}

void NearEnemy::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	//当たり判定で敵と背景との衝突の場合
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
	if (mHitBox == hitThisBox &&
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

void NearEnemy::HitAttackPlayer(BoxCollider* playerBox, BoxCollider* enemyBox)
{
	//敵に攻撃が当たったとき
	if (playerBox->GetType() == EnumPhysicsType::EnumPlayer &&
		enemyBox->GetType() == EnumPhysicsType::EnumEnemyAttack)
	{
		//当たったときにすぐ消す
		RemoveEnemyAttackHitBox();
	}
}

void NearEnemy::SetEnemyAttackHitBox()
{
	//プレイヤー攻撃用ボックス設置
	AABB EnemyAttackBox;

	mEnemyAttackBox = new BoxCollider(this, EnumPhysicsType::EnumEnemyAttack);

	EnemyAttackBox.mMin.x = 10;
	EnemyAttackBox.mMin.y = -50;
	EnemyAttackBox.mMin.z = 100;

	EnemyAttackBox.mMax.x = EnemyAttackBox.mMin.x + 150.0f;
	EnemyAttackBox.mMax.y = EnemyAttackBox.mMin.y + 150.0f;
	EnemyAttackBox.mMax.z = EnemyAttackBox.mMin.z + 150.0f;
	mEnemyAttackBox->SetObjectBox(EnemyAttackBox);
	mEnemyAttackBox->OnUpdateWorldTransform();
}

void NearEnemy::RemoveEnemyAttackHitBox()
{
	//近接敵の攻撃ボックスが敵に当たったら
	if (mEnemyAttackBox)
	{
		//攻撃のボックスを消す
		delete mEnemyAttackBox;
		mEnemyAttackBox = nullptr;
	}
}

bool NearEnemy::IsFrontHit()
{
	return mFrontTriggerBox->IsTrigerHit();
}
