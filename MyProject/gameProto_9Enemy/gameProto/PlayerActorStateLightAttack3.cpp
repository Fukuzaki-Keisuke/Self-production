#include "PlayerActorStateLightAttack3.h"
#include "Input.h"
#include "PlayerActor.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"

PlayerActorStateLightAttack3::PlayerActorStateLightAttack3()
	:mElapseTime(false)
	, mTotalAnimTime(0.0f)
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateAttack3\n");
}

PlayerActorStateLightAttack3::~PlayerActorStateLightAttack3()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateAttack3\n");
}

PlayerState PlayerActorStateLightAttack3::Update(PlayerActor* owner, float deltaTime)
{
	// アニメーションが終了したらcStopTime硬直後、IDLE状態へ
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		owner->RemoveAttackHitBox();
		return PlayerState::PLAYER_STATE_IDLE;
	}

	//攻撃踏み込みのためのアニメーション再生時間の経過割合を計算
	mElapseTime += deltaTime;
	const float speed = 5.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	//経過時間とともに移動処理
	Vector3 pos, forward;
	pos = owner->GetPosition();
	forward = owner->GetForward();
	pos += Math::EaseIn(percentage, speed, 0.0f) * forward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_LIGHTATTACK3;
}

void PlayerActorStateLightAttack3::Enter(PlayerActor* owner, float deltaTime)
{
	// アイドル状態のアニメーション再生
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK3));

	//アニメーションの再生時間取得
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK3)->GetDuration();
	mElapseTime = 0.0f;

	owner->SetAttackHitBox();
}

void PlayerActorStateLightAttack3::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
