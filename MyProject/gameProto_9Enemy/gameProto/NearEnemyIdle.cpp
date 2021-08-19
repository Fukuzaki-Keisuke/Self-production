#include "EnemyActorBase.h"
#include "NearEnemyIdle.h"

NearEnemyIdle::NearEnemyIdle(EnemyBehaviorComponent* owner)
	:EnemyState(owner)
{
	mStateType = EnemyStateEnum::Idle;
}

NearEnemyIdle::~NearEnemyIdle()
{
}

EnemyStateEnum NearEnemyIdle::Update(float deltaTime)
{
	// アニメーション再生終わったら歩きモードに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}
	// 続行
	return EnemyStateEnum::Idle;
}

void NearEnemyIdle::OnEnter()
{
	//Idleアニメーション再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::Idle);
}

void NearEnemyIdle::OnExit()
{
}
