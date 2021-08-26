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
	float		mWalkSpeed;        // �������x
	float		mWalkRemainTime;   // �����c�莞��

	const float mWalkTime = 3.0f;  // ��������
};