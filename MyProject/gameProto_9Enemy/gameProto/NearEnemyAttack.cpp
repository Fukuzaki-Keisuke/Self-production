#include "NearEnemyAttack.h"
#include "NearEnemy.h"

NearEnemyAttack::NearEnemyAttack(EnemyBehaviorComponent* owner)
	:EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack1;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

NearEnemyAttack::~NearEnemyAttack()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum NearEnemyAttack::Update(float deltaTime)
{
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Chase;
	}

	return EnemyStateEnum::Attack1;
}

void NearEnemyAttack::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack1);
}

void NearEnemyAttack::OnExit()
{
	
}