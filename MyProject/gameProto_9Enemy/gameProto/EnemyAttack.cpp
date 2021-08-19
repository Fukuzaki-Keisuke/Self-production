#include "EnemyAttack.h"
#include "EnemyShot.h"

EnemyAttack::EnemyAttack(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack1;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);

	//弾の上限
	mShotCount = 6;

	//今の弾のカウントにセット
	mNowShotCount = mShotCount;

}

EnemyAttack::~EnemyAttack()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum EnemyAttack::Update(float deltaTime)
{
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Chase;
	}

	//アニメーション中に弾を出す

	//弾が出るまでの時間を減算していく
	mShootTimer -= deltaTime;

	////弾を上限まで撃ったらリセット
	if (mNowShotCount == 0)
	{
		//次のカウントをセットするまでの時間
		mShootTimer = 5.0f;

		//弾の上限を再セット
		mNowShotCount = mShotCount;
	}

	if (mShootTimer < 0.0f)
	{
		//弾を呼び出す
		EnemyShot* es = new EnemyShot;
		//弾の位置の更新
		es->SetPosition(mOwnerActor->GetPosition());
		//弾の位置の補正
		es->SetShot(mOwnerActor->GetPosition());
		//プレイヤーの位置の更新
		es->RotateToNewForward(mOwnerActor->GetForward());

		//弾のカウントを減らす
		mNowShotCount--;

		//次の弾を出すまでのインターバルを設定する
		mShootTimer = 0.5f;
	}


	return EnemyStateEnum::Attack1;
}

void EnemyAttack::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack1);
}

void EnemyAttack::OnExit()
{
}