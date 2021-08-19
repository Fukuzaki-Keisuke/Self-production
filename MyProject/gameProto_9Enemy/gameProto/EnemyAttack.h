#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class EnemyAttack : public EnemyState
{
public:
	EnemyAttack(class EnemyBehaviorComponent* owner);
	~EnemyAttack();
	EnemyStateEnum Update(float deltaTime) override;
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