#include "PlayerActorStateJumpEnd.h"
#include "SkeletalMeshComponent.h"
#include "game.h"
#include "AudioManager.h"

PlayerActorStateJumpEnd::PlayerActorStateJumpEnd()
{
}

PlayerActorStateJumpEnd::~PlayerActorStateJumpEnd()
{
}

PlayerState PlayerActorStateJumpEnd::Update(PlayerActor* owner, float deltaTime)
{
	if (owner->GetSkeletalMeshComp()->IsPlaying())
	{
		return PlayerState::PLAYER_STATE_JUMPEND;
	}
	// �Đ��I���Ȃ�A�C�h����Ԃ�
	return PlayerState::PLAYER_STATE_IDLE;
}

void PlayerActorStateJumpEnd::Enter(PlayerActor* owner, float delataTime)
{
	// �A�j���[�V�������W�����v�J�n��
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_JUMPEND));
	AUDIO->PlaySound("assets/audio/jump.wav");
}
