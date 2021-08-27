#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyAttack : public EnemyState
{
public:
	NearEnemyAttack(class  EnemyBehaviorComponent* owner);
	~NearEnemyAttack();

	EnemyStateEnum Update(float deltaTime)override;
	void           OnEnter() override;
	void           OnExit() override;

private:
	class EnemyShot* mShot;
	float mElapseTime;
	float mTotalAnimTime;

	//弾を出すまでの時間
	float mShootTimer;
	//弾のカウント
	int	mShotCount;
	//現在の弾のカウント
	int mNowShotCount;
};