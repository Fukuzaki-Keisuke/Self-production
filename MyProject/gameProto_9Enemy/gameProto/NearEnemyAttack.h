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

	//�e���o���܂ł̎���
	float mShootTimer;
	//�e�̃J�E���g
	int	mShotCount;
	//���݂̒e�̃J�E���g
	int mNowShotCount;
};