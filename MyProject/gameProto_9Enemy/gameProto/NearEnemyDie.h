#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyDie : public EnemyState
{
public:
	NearEnemyDie(class EnemyBehaviorComponent* owner);
	~NearEnemyDie();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit() override;

private:
	float mElapseTime;
	float mTotalAnimTime;
	float mWaitTime;
};