#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateLightAttack2 :public PlayerActorStateBase
{
public:
	PlayerActorStateLightAttack2();
	~PlayerActorStateLightAttack2();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void Enter(class PlayerActor* owner, float deltaTime)override;
	void Exit(class PlayerActor* owner, float deltaTime)override;

private:
	bool  mIsNextCombo; // ���̃R���{�ɂȂ��邩  
	float mElapseTime; // ���̃X�e�[�g�ɓ����Ă���̌o�ߎ���
	float mTotalAnimTime; // �A�j���[�V����������
};