#pragma once
#include "Game.h"
#include "BoxCollider.h"

enum class EnumPhysicsType
{
	EnumEnemy,                // エネミー
	EnumPlayer,				  // プレーヤー
	EnumBG,					  // 背景ブロック
	EnumCoin,				  // コイン
	EnumHitCheck,		      // ヒットチェック用（押し返しなし)
	EnumBGTrigger,            // 背景と接触したかのトリガー
	EnumPlayerAttack,		  // プレイヤー攻撃判定
	EnumEnemyAttack,		  // エネミー攻撃班芸
	EnumEnemyShot			  // 敵の弾の当たり判定
};

class PhysicsWorld
{
public:

	PhysicsWorld();
	~PhysicsWorld();
	void AddBoxCollider(EnumPhysicsType type, class BoxCollider* box); // BoxCollider(AABB)追加
	void RemoveBoxCollider(class BoxCollider* box);                    // BoxCollider削除
	void DebugShowBoxLists();                                          // ボックスリスト表示（デバッグ用)
	void Collision();                                                  // コリジョン
	void DebugShowBox();                                               // デバッグ用ボックス表示
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }       // デバッグモード

private:
	bool                              mBoolDebugMode ;                 // デバッグモード
	std::vector<class BoxCollider*>   mBGBoxs        ;                 // 背景あたりデータ
	std::vector<class BoxCollider*>   mPlayerAttackBox;				   //プレイヤーの攻撃あたりデータ
	std::vector<class BoxCollider*>   mPlayerBoxs    ;                 // プレーヤーあたりデータ
	std::vector<class BoxCollider*>   mCoins         ;                 // コインあたりデータ

	std::vector<class BoxCollider*>   mEnemies;					       // 敵キャラ             
	std::vector<class BoxCollider*>   mBGTriggers;                     // 背景との接触判定用トリガー

	std::vector<class BoxCollider*>   mEnemyAttackBox;				   //プレイヤーの攻撃あたりデータ
	std::vector<class BoxCollider*>   mEnemyShotBox;				   //敵弾のあたりデータ

	void PlayerAndBGTest();  // プレーヤーと壁とのあたり判定
	void EnemyAndBGTest();   // 敵と壁との当たり判定
	void TriggerAndBGTest(); // 背景トリガーと背景の判定テスト
	void InitBoxVertices();
	void PlayerAttackAndEnemyTest();	//プレイヤーの攻撃ボックスとエネミーの当たり判定
	void EnemyAttackBoxAndPlayerTest();    //エネミーの攻撃ボックスとプレイヤーの当たり判定
	void EnemyAttackAndPlayerTest();	//敵弾とプレイヤーの当たり判定
	void PlayerABandEnemyBBTest();		//プレイヤーの攻撃ボックスと敵の弾の当たり判定
	void EnemyBBTestandBGTest();		//敵の弾と壁との当たり判定
	void DrawBoxs(std::vector<class BoxCollider*>& boxs, const Vector3& color);
	unsigned int mBoxVAO; // ボックス描画用のVAO  
	class Shader* mLineShader; // ライン描画用シェーダー
};