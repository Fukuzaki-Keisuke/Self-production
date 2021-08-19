#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateHIt : public PlayerActorStateBase
{
public:
	PlayerActorStateHIt();
	~PlayerActorStateHIt();
	PlayerState Update(PlayerActor* owner, float deltaTime) override;
	void Enter(class PlayerActor* owner, float deltaTime)override;
private:
	float mElapseTime;
	float mTotalAnimTime;
};