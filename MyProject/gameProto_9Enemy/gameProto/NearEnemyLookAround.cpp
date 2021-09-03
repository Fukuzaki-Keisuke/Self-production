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
	// �A�j���[�V�����Đ��I�������������[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Walk;
	}
	// ���s
	return EnemyStateEnum::LookAround;
}

void NearEnemyLookAround::OnEnter()
{
	// ���n���A�j���Đ�
	mOwnerActor->PlayAnimation(EnemyStateEnum::LookAround);
}

void NearEnemyLookAround::OnExit()
{
}
