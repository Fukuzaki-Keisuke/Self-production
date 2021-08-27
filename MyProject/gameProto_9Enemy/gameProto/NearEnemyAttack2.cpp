#include "NearEnemyAttack2.h"

NearEnemyAttack2::NearEnemyAttack2(EnemyBehaviorComponent* owner)
	:EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack2;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

NearEnemyAttack2::~NearEnemyAttack2()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum NearEnemyAttack2::Update(float deltaTime)
{
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Chase;
	}

	return EnemyStateEnum::Attack2;
}

void NearEnemyAttack2::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack2);
}

void NearEnemyAttack2::OnExit()
{
}
