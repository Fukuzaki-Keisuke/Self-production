#include "NearEnemyLookAround.h"

NearEnemyLookAround::NearEnemyLookAround(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::LookAround;
}

NearEnemyLookAround::~NearEnemyLookAround()
{
}

EnemyStateEnum NearEnemyLookAround::Update(float deltaTime)
{
	// アニメーション再生終わったら歩きモードに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}
	// 続行
	return EnemyStateEnum::LookAround;
}

void NearEnemyLookAround::OnEnter()
{
	// 見渡しアニメ再生
	mOwnerActor->PlayAnimation(EnemyStateEnum::LookAround);
}

void NearEnemyLookAround::OnExit()
{
}
