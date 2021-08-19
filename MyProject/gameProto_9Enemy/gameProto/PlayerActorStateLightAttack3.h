#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateLightAttack3 :public PlayerActorStateBase
{
public:
	PlayerActorStateLightAttack3();
	~PlayerActorStateLightAttack3();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void Enter(class PlayerActor* owner, float deltaTime)override;
	void Exit(class PlayerActor* owner, float deltaTime)override;

private:
	float mElapseTime; // ���̃X�e�[�g�ɓ����Ă���̌o�ߎ���
	float mTotalAnimTime; // �A�j���[�V����������
};