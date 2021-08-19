#include "EnemyDeath.h"

EnemyDeath::EnemyDeath(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Die;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyDeath::~EnemyDeath()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum EnemyDeath::Update(float deltaTime)
{
	return EnemyStateEnum::Die;
}

void EnemyDeath::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Die);
}

void EnemyDeath::OnExit()
{
}
