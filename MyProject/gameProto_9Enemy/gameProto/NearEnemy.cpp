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
#include "NearEnemyWalk.h"
#include "NearEnemyChase.h"

#include <iostream>

NearEnemy::NearEnemy()
{
	//初期パラメータ
	mWalkSpeed = 100.0f;
	mRunSpeed = 200.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 1;

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

	// EnemyBehaviorComponentにふるまいを追加
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new NearEnemyIdle(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyWalk(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyChase(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Walk);

	//敵キャラの当たり判定を追加
	AABB enemyBox = mesh->GetCollisionBox();
	enemyBox.mMin.x *= 1.5f;
	enemyBox.mMax.x *= 1.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(enemyBox);
	mHitBox->SetArrowRotate(false);

	//エネミーの近接攻撃ボックス
	AABB enemyForward;
	enemyForward.mMin.x = enemyBox.mMax.x;
	enemyForward.mMin.y = enemyBox.mMin.y;
	enemyForward.mMin.z = enemyBox.mMin.z + 100;
	enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;
	SetTriggerBox(EnemyTriggerEnum::ForwardBox, enemyForward);

}

NearEnemy::~NearEnemy()
{
	std::cout << "NearEnemy破棄" << mID << this << std::endl;
}

void NearEnemy::UpdateActor(float deltaTime)
{
}

void NearEnemy::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
}

//bool NearEnemy::IsFrontHit()
//{
//	return mFrontTriggerBox->IsTrigerHit();
//}
