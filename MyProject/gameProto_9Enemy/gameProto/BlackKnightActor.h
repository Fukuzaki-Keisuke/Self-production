#pragma once
#include "EnemyActorBase.h"
#include "BoxCollider.h"

class BlackKnightActor : public EnemyActorBase
{
public:
	BlackKnightActor();
	~BlackKnightActor();

	void UpdateActor(float deltaTime) override;
	void OnCollision(BoxCollider* hitThisBox, class BoxCollider* hitOtherBox) override;

	bool IsFrontHit();



private:
	
	
	class BoxCollider* mHitBox;
	class BoxCollider* mHitGroundBox;						//ínñ ÇÃê›íu
	class BoxCollider* mFrontTriggerBox;
	class EnemyBehaviorComponent* mEnemyBehaviorComponent;
};