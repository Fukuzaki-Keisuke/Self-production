#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class EnemyChase : public EnemyState
{
public:
	EnemyChase(class EnemyBehaviorComponent* owner);
	~EnemyChase();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit() override;

private:
	float		mAngleSpeed;       // ターンスピード
	float		mWalkSpeed;        // 歩き速度
	float		mWalkRemainTime;   // 歩き残り時間
	
	const int   mTurnMaxNum = 5;   // 最大ターン回数→見わたし行動に移行
	const float mWalkTime = 3.0f;  // 歩き時間
};