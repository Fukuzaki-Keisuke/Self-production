#pragma once
#include "EnemyActorBase.h"
#include "BoxCollider.h"

class  NearEnemy : public EnemyActorBase
{
public:
	 NearEnemy();
	~ NearEnemy();

	void UpdateActor(float deltaTime)override;
	void OnCollision(BoxCollider* hitThisBox, class BoxCollider* hitOtherBox) override;

	bool IsFrontHit();

private:

	class BoxCollider* mHitBox;
	class BoxCollider* mHitGroundBox;
	class BoxCollider* mFrontTriggerBox;
	class EnemyBehaviorComponent* mEnemyBehaviorComponent;
};