#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateLightAttack1 :public PlayerActorStateBase
{
public:
	PlayerActorStateLightAttack1();
	~PlayerActorStateLightAttack1();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void Enter(class PlayerActor* owner, float deltaTime)override;

	//�U���{�b�N�X������
	void Exit(class PlayerActor* owner, float deltaTime)override;

private:
	bool  mIsNextCombo; // ���̃R���{�ɂȂ��邩  
	float mElapseTime; // ���̃X�e�[�g�ɓ����Ă���̌o�ߎ���
	float mTotalAnimTime; // �A�j���[�V����������
};