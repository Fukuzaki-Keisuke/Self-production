#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyChase : public EnemyState
{
public:
	NearEnemyChase(class EnemyBehaviorComponent* owner);
	~NearEnemyChase();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit() override;

private:
	float		mWalkSpeed;        // 歩き速度
	float		mWalkRemainTime;   // 歩き残り時間

	const float mWalkTime = 3.0f;  // 歩き時間
};