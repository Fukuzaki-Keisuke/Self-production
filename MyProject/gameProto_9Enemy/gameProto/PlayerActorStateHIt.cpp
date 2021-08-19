#include "PlayerActorStateHIt.h"
#include "Input.h"
#include "PlayerActor.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"

PlayerActorStateHIt::PlayerActorStateHIt()
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateHit\n");
}

PlayerActorStateHIt::~PlayerActorStateHIt()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateHit\n");
}

PlayerState PlayerActorStateHIt::Update(PlayerActor* owner, float deltaTime)
{
	// アニメーションが終了したらcStopTime硬直後、IDLE状態へ
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		return PlayerState::PLAYER_STATE_IDLE;
	}

	return PlayerState::PLAYER_STATE_HIT;
}

void PlayerActorStateHIt::Enter(PlayerActor* owner, float deltaTime)
{
	// アイドル状態のアニメーション再生
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_HIT));
}
