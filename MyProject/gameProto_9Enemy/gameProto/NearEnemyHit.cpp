#include "NearEnemyHit.h"

NearEnemyHit::NearEnemyHit(EnemyBehaviorComponent* owner)
	:EnemyState(owner)
{
	mStateType = EnemyStateEnum::GetDamage;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

NearEnemyHit::~NearEnemyHit()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum NearEnemyHit::Update(float deltaTime)
{
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Chase;
	}

	return EnemyStateEnum::GetDamage;
}

void NearEnemyHit::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::GetDamage);
}

void NearEnemyHit::OnExit()
{
}
