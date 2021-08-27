#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyAttack2 : public EnemyState
{
public:
	NearEnemyAttack2(class  EnemyBehaviorComponent* owner);
	~NearEnemyAttack2();
	EnemyStateEnum Update(float deltaTime)override;
	void           OnEnter() override;
	void           OnExit() override;

private:

};