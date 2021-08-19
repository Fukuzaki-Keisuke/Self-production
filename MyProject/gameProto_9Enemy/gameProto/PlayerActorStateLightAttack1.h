#pragma once
#include "PlayerActorStateBase.h"

class PlayerActorStateLightAttack1 :public PlayerActorStateBase
{
public:
	PlayerActorStateLightAttack1();
	~PlayerActorStateLightAttack1();
	PlayerState Update(PlayerActor* owner, float deltaTime)override;
	void Enter(class PlayerActor* owner, float deltaTime)override;

	//攻撃ボックスを消す
	void Exit(class PlayerActor* owner, float deltaTime)override;

private:
	bool  mIsNextCombo; // 次のコンボにつなげるか  
	float mElapseTime; // このステートに入ってからの経過時刻
	float mTotalAnimTime; // アニメーション総時間
};