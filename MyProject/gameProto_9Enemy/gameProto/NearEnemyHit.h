#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyHit : public EnemyState
{
public:
	NearEnemyHit(class EnemyBehaviorComponent* owner);
	~NearEnemyHit();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit() override;

private:
	float mElapseTime;
	float mTotalAnimTime;
};