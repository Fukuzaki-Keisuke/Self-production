#include "NearEnemyAttack.h"
#include "EnemyShot.h"

NearEnemyAttack::NearEnemyAttack(EnemyBehaviorComponent* owner)
	:EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack1;
	printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);

	//�e�̏��
	mShotCount = 1;

	//���̒e�̃J�E���g�ɃZ�b�g
	mNowShotCount = mShotCount;
}

NearEnemyAttack::~NearEnemyAttack()
{
	printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum NearEnemyAttack::Update(float deltaTime)
{
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return EnemyStateEnum::Chase;
	}

	//�A�j���[�V�������ɒe���o��

	//�e���o��܂ł̎��Ԃ����Z���Ă���
	mShootTimer -= deltaTime;

	////�e������܂Ō������烊�Z�b�g
	if (mNowShotCount == 0)
	{
		//���̃J�E���g���Z�b�g����܂ł̎���
		mShootTimer = 2.0f;

		//�e�̏�����ăZ�b�g
		mNowShotCount = mShotCount;
	}

	if (mShootTimer < 0.0f)
	{
		//�e���Ăяo��
		EnemyShot* es = new EnemyShot;
		//�e�̈ʒu�̍X�V
		es->SetPosition(mOwnerActor->GetPosition());
		//�e�̈ʒu�̕␳
		es->SetShot(mOwnerActor->GetPosition());
		//�v���C���[�̈ʒu�̍X�V
		es->RotateToNewForward(mOwnerActor->GetForward());

		//�e�̃J�E���g�����炷
		mNowShotCount--;

		//���̒e���o���܂ł̃C���^�[�o����ݒ肷��
		mShootTimer = 0.5f;
	}

	return EnemyStateEnum::Attack1;
}

void NearEnemyAttack::OnEnter()
{
	mOwnerActor->PlayAnimation(EnemyStateEnum::Attack1);
}

void NearEnemyAttack::OnExit()
{
	
}