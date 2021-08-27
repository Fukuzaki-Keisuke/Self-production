#pragma once
#include "Actor.h"
#include "Collision.h"

enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,     // 待機
	PLAYER_STATE_RUN,          // 走る
	PLAYER_STATE_JUMPSTART,    // ジャンプスタート
	PLAYER_STATE_JUMPLOOP,     // ジャンプループ
	PLAYER_STATE_JUMPEND,      // ジャンプ終了
	PLAYER_STATE_LIGHTATTACK1, // 弱攻撃1
	PLAYER_STATE_LIGHTATTACK2, // 弱攻撃2
	PLAYER_STATE_LIGHTATTACK3, // 弱攻撃3
	PLAYER_STATE_HIT,		   // ダメージ

	PLAYER_STATE_NUM,   // 総アニメーション数
};

// 自機トリガーボックス種類
enum class PlayerTriggerEnum : unsigned char
{
	ForwardBox,          // 正面
	LeftBox,             // 左側
	RightBox,            // 右
	BackBox,             // 後ろ
	HeadBox,             // 頭上
	GroundBox,           // 足元
	GroundForwardBox     // 足元前方
};


class PlayerActor : public Actor
{
public:
	PlayerActor();
	~PlayerActor();

	void UpdateActor(float deltaTime) override;
	void									 FixCollision(class BoxCollider *hitPlayerBox, class BoxCollider *hitBox);
	void									 FixEnemyCollision(class BoxCollider* hitPlayerBox, class BoxCollider* enemyBox);
	class SkeletalMeshComponent*			 GetSkeletalMeshComp();
	const class Animation*					 GetAnim(PlayerState state);
	bool									 GetIsJump()const { return mIsJump; }
	bool									 GetOnground() const { return mOnGroundChk; }
	Vector3&								 GetJumpVec() { return mJumpVec; }

	void									 SetOnground(bool onGround) { mOnGroundChk = onGround; }
	void									 SetIsJump(bool jump) { mIsJump = jump; }
	void									 SetJumpVec(Vector3& vec) { mJumpVec = vec; }

	//体力
	void									 SetmPHitPoint(int hitpoint) { mPHitPoint = hitpoint; }  // ヒットポイントセット
	int										 GetmPHitPoint() const { return mPHitPoint; }  // 体力の取得


	//敵との当たり判定関係
	void									 HitEnemy(class BoxCollider* playerBox, class BoxCollider* enemyBox); //対象との当たり判定
	void									 SetAttackHitBox();		//当たり判定の付与
	void									 RemoveAttackHitBox();	//当たり判定の消去

	//敵の近接攻撃との当たり判定
	void									 HitEnemyAttackDamage(class BoxCollider* hitPlayerBox, class BoxCollider* mEnemyAttackBox);

	//敵の弾との当たり判定
	void									 HitBullet(class BoxCollider* hitPlayerBox, class BoxCollider* mEnemyShotBox);	//対象との当たり判定の付与

	//敵の弾と攻撃ボックスの当たり判定
	void									 PlayerABandEnemyBB(class BoxCollider* mPlayerAttackBox, class BoxCollider* mEnemyShotBox);


private:
	class SkeletalMeshComponent*             mMeshComp;
	std::vector<const class Animation*>      mAnimTypes;
	class BoxCollider*                       mHitBox;
	class BoxCollider*                       mHitGroundBox;
	class BoxCollider*                       mHitHeadBox;

	//攻撃用当たり判定ボックス
	class BoxCollider*						 mAttackBox;	   // プレイヤーの攻撃当たり判定ボックス

	PlayerState                              mNowState;        // 現在のステート
	PlayerState                              mNextState;       // 次のステート
	std::vector<class PlayerActorStateBase*> mStatePools;      // ステートクラスプール

	int										 mPHitPoint;	   // 体力

	bool                                     mIsJump;
	bool                                     mOnGroundChk;
	Vector3                                  mJumpVec;
	Vector3                                  mVelocityVec;
};