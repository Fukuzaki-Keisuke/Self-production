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
	float		mAngleSpeed;       // �^�[���X�s�[�h
	float		mWalkSpeed;        // �������x
	float		mWalkRemainTime;   // �����c�莞��
	
	const int   mTurnMaxNum = 5;   // �ő�^�[���񐔁����킽���s���Ɉڍs
	const float mWalkTime = 3.0f;  // ��������
};