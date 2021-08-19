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
	// �A�j���[�V�������I��������cStopTime�d����AIDLE��Ԃ�
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		owner->RemoveAttackHitBox();
		return PlayerState::PLAYER_STATE_IDLE;
	}

	//�U�����ݍ��݂̂��߂̃A�j���[�V�����Đ����Ԃ̌o�ߊ������v�Z
	mElapseTime += deltaTime;
	const float speed = 5.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	//�o�ߎ��ԂƂƂ��Ɉړ�����
	Vector3 pos, forward;
	pos = owner->GetPosition();
	forward = owner->GetForward();
	pos += Math::EaseIn(percentage, speed, 0.0f) * forward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_LIGHTATTACK3;
}

void PlayerActorStateLightAttack3::Enter(PlayerActor* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshcomp = owner->GetSkeletalMeshComp();
	meshcomp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK3));

	//�A�j���[�V�����̍Đ����Ԏ擾
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK3)->GetDuration();
	mElapseTime = 0.0f;

	owner->SetAttackHitBox();
}

void PlayerActorStateLightAttack3::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
