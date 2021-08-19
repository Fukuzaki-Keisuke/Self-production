#include "EnemyHit.h"

EnemyHit::EnemyHit(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::GetDamage;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyHit::~EnemyHit()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum EnemyHit::Update(float deltaTime)
{
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Chase;
	}

	return EnemyStateEnum::GetDamage;
}

void EnemyHit::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::GetDamage);
}

void EnemyHit::OnExit()
{
}