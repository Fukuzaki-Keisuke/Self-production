#include "NearEnemyDie.h"

NearEnemyDie::NearEnemyDie(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Die;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

NearEnemyDie::~NearEnemyDie()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum NearEnemyDie::Update(float deltaTime)
{
	return EnemyStateEnum::Die;
}

void NearEnemyDie::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Die);
}

void NearEnemyDie::OnExit()
{
}
