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
	Vector3 mAimDir;        // �ڕW�����x�N�g��
	Vector3 mPrevAimDir;        // �ȑO�̖ڕW�����x�N�g��
	bool    mTurnMode;        // �^�[�������H
	float   mAngleSpeed;        // �^�[���X�s�[�h
	float   mWalkSpeed;        // �������x
	float   mWalkRemainTime;        // �����c�莞��
	int     mTurnNum;        // ����񐔃J�E���^

	const int   mTurnMaxNum = 5; // �ő�^�[���񐔁�Idle
	const float mWalkTime =	3.0f; // ��������
};