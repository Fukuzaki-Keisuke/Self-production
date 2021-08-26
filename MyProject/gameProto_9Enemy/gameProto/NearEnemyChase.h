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
	float		mWalkSpeed;        // •à‚«‘¬“x
	float		mWalkRemainTime;   // •à‚«Žc‚èŽžŠÔ

	const float mWalkTime = 3.0f;  // •à‚«ŽžŠÔ
};