#pragma once
#include "PlayerActor.h"

class PlayerActorStateBase
{
public:
	PlayerActorStateBase() {};
	virtual ~PlayerActorStateBase() {};
	virtual PlayerState Update(class PlayerActor* owner, float deltaTime) = 0;
	virtual void Enter(class PlayerActor* owner, float deltaTime) {};
	virtual void Exit(class PlayerActor* owner, float deltaTime) {};

	//�v���C���[�̃A�j���[�V�����t���O(�U��)
	bool plAkFlg;
};