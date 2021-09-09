#include "BlackKnightActor.h"

#include "game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Input.h"
#include "GameClearScene.h"

#include "EnemyBehaviorComponent.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyHit.h"
#include "EnemyDeath.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemyShot.h"

#include <iostream>

BlackKnightActor::BlackKnightActor()
{
	// �����p�����[�^�ݒ�
	mWalkSpeed  = 100.0f;
	mRunSpeed   = 200.0f;
	mTurnSpeed  = Math::Pi;
	mHitPoint   = 2;

	//�n�ʂƂ̐ݒu����
	mIsOnGround = true;

	// ���f���ǂݍ���
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/BlackKnightSK.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Enemy/BlackKnightSK.gpskel"));

	// �A�j���[�V�����ǂݍ���
	mAnimations.emplace(EnemyStateEnum::Walk,       RENDERER->GetAnimation("Assets/Enemy/BlackKnight_WalkFWDAnim.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::LookAround, RENDERER->GetAnimation("Assets/Enemy/BlackKnight_SenseSomethingRPTAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::GetDamage,  RENDERER->GetAnimation("Assets/Enemy/BlackKnight_GetHitAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Die,	    RENDERER->GetAnimation("Assets/Enemy/BlackKnight_DieAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Chase,		RENDERER->GetAnimation("Assets/Enemy/BlackKnight_WalkFWDAnim.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Attack1,    RENDERER->GetAnimation("Assets/Enemy/BlackKnight_Attack01Anim.gpanim", false));

	// EnemyBehaviorComponent �� �ӂ�܂���ǉ�
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new EnemyPatrol(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyLookAround(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyHit(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyDeath(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyChase(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new EnemyAttack(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Walk);

	// �G�L�����̓����蔻���ǉ�
	AABB enemyBox = mesh->GetCollisionBox();
	enemyBox.mMin.x *= 0.5f;
	enemyBox.mMax.x *= 0.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(enemyBox);
	mHitBox->SetArrowRotate(false);

	//�G�l�~�[�̋ߐڍU���p�{�b�N�X
	AABB enemyForward;
	enemyForward.mMin.x = enemyBox.mMax.x;
	enemyForward.mMin.y = enemyBox.mMin.y;
	enemyForward.mMin.z = enemyBox.mMin.z + 100;
	enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;
	SetTriggerBox(EnemyTriggerEnum::ForwardBox, enemyForward);
}	

BlackKnightActor::~BlackKnightActor()
{
	std::cout << "BlackKnight�j��" << mID << this << std::endl;
}

void BlackKnightActor::UpdateActor(float deltaTime)
{

	// BlackKnight�̍s����EnemyBehaviorComponent�Ɉ�C

	if (IsHitTrigger(EnemyTriggerEnum::ForwardBox))
	{
		std::cout << "ForwardBoxHit!!" << std::endl;
	}

	//�̗͂�0�ɂȂ�����{�b�N�X������
	if (mHitPoint <= 0)
	{
		mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::Die);
		delete mHitBox;

		//�G�l�~�[������
		SetState(EDead);

		//�|������N���A
		//�c�@��1���炷
		//mEnemyCount--;
	}

	//�����{�^��
	//�L�[Z
	/*if (INPUT_INSTANCE.GetInput(KEY_A))
	{
		mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::Die);
		mRemaining--;
	}*/

}

void BlackKnightActor::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	// �����蔻��ŋA���Ă������ʂ�mHitBox�A�w�i�Ƃ̏Փ˂������ꍇ
	if (mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumBG)
	{
		//�ǂƃG�l�~�[�ɂԂ������Ƃ�
		AABB bgBox = hitOtherBox->GetWorldBox();
		AABB enemyBox = hitThisBox->GetWorldBox();
		Vector3 fixVec;

		// �߂荞�݂��C��
		calcCollisionFixVec(enemyBox, bgBox, fixVec);
		mPosition += fixVec;
		//�ʒu���ς�����̂Ń{�b�N�X���Čv�Z
		mHitBox->OnUpdateWorldTransform();
	}

	//�v���C���[�̍U�����q�b�g
	if(mHitBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayerAttack)
	{

		if (mNowState != EnemyStateEnum::GetDamage && mNowState != EnemyStateEnum::Die)
		{

			// �̗͂̌��Z
			mHitPoint -= 1;
			SetHitPoint(mHitPoint);
			printf("�U��������������\n");
			mEnemyBehaviorComponent->ChangeState(EnemyStateEnum::GetDamage);
		}
	}
}

bool BlackKnightActor::IsFrontHit()
{
	return mFrontTriggerBox->IsTrigerHit();
}
