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
	// �A�j���[�V�����Đ��I�������������[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}
	// ���s
	return EnemyStateEnum::Idle;
}

void NearEnemyIdle::OnEnter()
{
	//Idle�A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::Idle);
}

void NearEnemyIdle::OnExit()
{
}
