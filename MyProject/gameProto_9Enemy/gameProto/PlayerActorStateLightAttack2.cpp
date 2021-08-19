#include "PlayerActorStateLightAttack2.h"
#include "Input.h"
#include "PlayerActor.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"

PlayerActorStateLightAttack2::PlayerActorStateLightAttack2()
	: mIsNextCombo(false)
	, mElapseTime(0.0f)
	, mTotalAnimTime(0.0f)
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateAttack2\n");
}

PlayerActorStateLightAttack2::~PlayerActorStateLightAttack2()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateAttack1\n");
}

PlayerState PlayerActorStateLightAttack2::Update(PlayerActor* owner, float deltaTime)
{
	// アニメーションが終了したらアイドル状態か、次のコンボへ
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		owner->RemoveAttackHitBox();
		if (mIsNextCombo)
		{

			return PlayerState::PLAYER_STATE_LIGHTATTACK3;
		}
		return PlayerState::PLAYER_STATE_IDLE;
	}

	// 攻撃ボタン押されたら次のステートへ移行する準備
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_CONFIG::KEY_B))
	{
		mIsNextCombo = true;
	}

	//攻撃踏み込み移動のためのアニメーション再生時間の経過割合を計算
	mElapseTime += deltaTime;
	const float speed = 5.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	//経過時間とともに移動処理
	Vector3 pos, forward;
	pos = owner->GetPosition();
	forward = owner->GetForward();
	pos += Math::EaseIn(percentage, speed, 0.0f) * forward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_LIGHTATTACK2;
}

void PlayerActorStateLightAttack2::Enter(PlayerActor* owner, float deltaTime)
{
	// ATTACK1のアニメーション再生
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK2));
	mIsNextCombo = false;

	//アニメーションの再生時間取得
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK2)->GetDuration();
	mElapseTime = 0.0f;

	//アタックボックスの配置
	owner->SetAttackHitBox();
}

void PlayerActorStateLightAttack2::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
