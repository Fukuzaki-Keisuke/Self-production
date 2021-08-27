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

	//プレイヤーとの当たり判定
	void						HitAttackPlayer(class BoxCollider* playerBox, class BoxCollider* enemyBox);
	void						SetEnemyAttackHitBox();		//当たり判定の付与
	void						RemoveEnemyAttackHitBox();	//当たり判定の消去

private:

	class BoxCollider* mHitBox;
	class BoxCollider* mHitGroundBox;	
	class EnemyBehaviorComponent* mEnemyBehaviorComponent;
	class BoxCollider* mFrontTriggerBox;
	//攻撃用当たり判定ボックス
	class BoxCollider* mEnemyAttackBox;	   // プレイヤーの攻撃当たり判定ボックス
};