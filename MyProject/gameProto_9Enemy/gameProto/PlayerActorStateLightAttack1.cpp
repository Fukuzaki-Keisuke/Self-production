#include "PlayerActorStateLightAttack1.h"
#include "Input.h"
#include "PlayerActor.h"
#include "SkeletalMeshComponent.h"
#include "Animation.h"

PlayerActorStateLightAttack1::PlayerActorStateLightAttack1()
	: mIsNextCombo(false)
	, mElapseTime(0.0f)
	, mTotalAnimTime(0.0f)
{
	printf("Create : [PlayerActorStateBase] PlayerActorStateAttack1\n");
}

PlayerActorStateLightAttack1::~PlayerActorStateLightAttack1()
{
	printf("Remove : [PlayerActorStateBase] PlayerActorStateAttack1\n");
}

PlayerState PlayerActorStateLightAttack1::Update(PlayerActor* owner, float deltaTime)
{
	// �A�j���[�V�������I��������A�C�h����Ԃ��A���̃R���{��
	if (!owner->GetSkeletalMeshComp()->IsPlaying())
	{
		owner->RemoveAttackHitBox();
		if (mIsNextCombo)
		{
			return PlayerState::PLAYER_STATE_LIGHTATTACK2;
		}
		return PlayerState::PLAYER_STATE_IDLE;
	}

	// �U���{�^�������ꂽ�玟�̃X�e�[�g�ֈڍs���鏀��
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_CONFIG::KEY_B))
	{
		mIsNextCombo = true;
	}

	//�U�����ݍ��݈ړ��̂��߂̃A�j���[�V�����Đ����Ԃ̌o�ߊ������v�Z
	mElapseTime += deltaTime;
	const float speed = 5.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	//�o�ߎ��ԂƂƂ��Ɉړ�����
	Vector3 pos, forward;
	pos = owner->GetPosition();
	forward = owner->GetForward();
	pos += Math::EaseIn(percentage, speed, 0.0f) * forward;

	owner->SetPosition(pos);

	return PlayerState::PLAYER_STATE_LIGHTATTACK1;
}

void PlayerActorStateLightAttack1::Enter(PlayerActor* owner, float deltaTime)
{
	// ATTACK1�̃A�j���[�V�����Đ�
	SkeletalMeshComponent* meshComp = owner->GetSkeletalMeshComp();
	meshComp->PlayAnimation(owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK1));
	mIsNextCombo = false;

	//�A�j���[�V�����̍Đ����Ԏ擾
	mTotalAnimTime = owner->GetAnim(PlayerState::PLAYER_STATE_LIGHTATTACK1)->GetDuration();
	mElapseTime = 0.0f;

	//�A�^�b�N�{�b�N�X�̔z�u
	owner->SetAttackHitBox();
}

void PlayerActorStateLightAttack1::Exit(PlayerActor* owner, float deltaTime)
{
	owner->RemoveAttackHitBox();
}
