#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class EnemyDeath : public EnemyState
{
public:
	EnemyDeath(class EnemyBehaviorComponent* owner);
	~EnemyDeath();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit() override;

private:
	float mElapseTime;
	float mTotalAnimTime;
	float mWaitTime;
};