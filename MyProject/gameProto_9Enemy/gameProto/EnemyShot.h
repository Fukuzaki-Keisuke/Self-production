#pragma once
#include "EnemyActorBase.h"
#include "EnemyBehaviorComponent.h"

class EnemyShot : public Actor
{
public:
	EnemyShot();
	~EnemyShot();
	void                         UpdateActor(float deltaTime) override;
	void                         FixShotCollision(class BoxCollider* hitThisBox, class BoxCollider* hiOtherBox);
	void						 HitShot();
	/*void						 ShotFront(Vector3 pos);*/
	void						 SetShot(Vector3 pos);
	/*void						 setPPos(Vector3 pos);*/

	//プレイヤーとの当たり判定
	void						 HitPlayer(class BoxCollider* playerBox, class BoxCollider* mEnemyShotBox);
	//void						 SetAttackBulletBox();		//当たり判定の付与
	void						 RemoveAttackBulletBox();	//当たり判定の消去

	//プレイヤーの攻撃と敵の弾との当たり判定
	void						 EnemyBBandPlayerAB(class BoxCollider* mEnemyShotBox,class BoxCollider* mPlayerAttackBox);

private:
	class MeshComponent* mMeshComp;
	class BoxCollider* mShotBox;
	class BlackKnightActor* Position;

	//攻撃用当たり判定ボックス
	class BoxCollider* mAttackBox; // プレイヤーの攻撃当たり判定ボックス

	//角度
	float mAngle;
	//弾の位置
	Vector3 mshotpos;

	Vector3 shotPos;
	bool TimerFlag;
	Vector3 mFront;
	bool	shotFlag;
	Vector3 mPPos;
};