#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyWalk : public EnemyState
{
public:
	NearEnemyWalk(EnemyBehaviorComponent* owner);
	~NearEnemyWalk();
	EnemyStateEnum Update(float deltaTime)override;
	void		   OnEnter()override;
	void           OnExit() override;

private:
	Vector3 mAimDir;        // 目標向きベクトル
	Vector3 mPrevAimDir;        // 以前の目標向きベクトル
	bool    mTurnMode;        // ターン中か？
	float   mAngleSpeed;        // ターンスピード
	float   mWalkSpeed;        // 歩き速度
	float   mWalkRemainTime;        // 歩き残り時間
	int     mTurnNum;        // 旋回回数カウンタ

	const int   mTurnMaxNum = 5; // 最大ターン回数→Idle
	const float mWalkTime =	3.0f; // 歩き時間
};