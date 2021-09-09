#include "NearEnemy.h"

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
#include "NearEnemyIdle.h"
#include "NearEnemyLookAround.h"
#include "NearEnemyWalk.h"
#include "NearEnemyChase.h"
#include "NearEnemyAttack.h"
#include "NearEnemyAttack2.h"
#include "NearEnemyHit.h"
#include "NearEnemyDie.h"

#include "EnemyShot.h"

#include <iostream>

NearEnemy::NearEnemy()
{
	//�����p�����[�^
	mWalkSpeed = 100.0f;
	mRunSpeed = 200.0f;
	mTurnSpeed = Math::Pi;
	mHitPoint = 5;

	//�n�ʂƂ̐ݒu����
	mIsOnGround = true;

	//���f���̓ǂݍ���
	mSkelMeshComponent = new SkeletalMeshComponent(this);
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/NearEnemy/NearEnemy_Idle.gpmesh");
	mSkelMeshComponent->SetMesh(mesh);
	mSkelMeshComponent->SetSkeleton(RENDERER->GetSkeleton("Assets/Enemy/NearEnemy/NearEnemy_Idle.gpskel"));

	//�A�j���[�V�����̓ǂݍ���
	//�Ō��true�̓��[�v���邩���Ȃ���
	mAnimations.emplace(EnemyStateEnum::Idle, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Idle_Anim.gpanim",false));
	mAnimations.emplace(EnemyStateEnum::Walk, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Walk1.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::Chase, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Walk2.gpanim", true));
	mAnimations.emplace(EnemyStateEnum::LookAround, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_LookAround.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Attack1, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Attack1.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Attack2, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Attack2.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::GetDamage, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Hit.gpanim", false));
	mAnimations.emplace(EnemyStateEnum::Die, RENDERER->GetAnimation("Assets/Enemy/NearEnemy/NearEnemy_Die.gpanim", false));

	// EnemyBehaviorComponent�ɂӂ�܂���ǉ�
	mEnemyBehaviorComponent = new EnemyBehaviorComponent(this);
	mEnemyBehaviorComponent->RegisterState(new NearEnemyIdle(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyWalk(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyChase(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyLookAround(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyAttack(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyAttack2(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyHit(mEnemyBehaviorComponent));
	mEnemyBehaviorComponent->RegisterState(new NearEnemyDie(mEnemyBehaviorComponent));

	mEnemyBehaviorComponent->SetFirstState(EnemyStateEnum::Walk);

	//�G�L�����̓����蔻���ǉ�
	AABB enemyBox = mesh->GetCollisionBox();
	enemyBox.mMin.x *= 1.5f;
	enemyBox.mMax.x *= 1.5f;
	mHitBox = new BoxCollider(this, EnumPhysicsType::EnumEnemy);
	mHitBox->SetObjectBox(enemyBox);
	mHitBox->SetArrowRotate(false);

	////�G�l�~�[�̋ߐڍU���{�b�N�X
	//AABB enemyForward;
	//enemyForward.mMin.x = enemyBox.mMax.x;
	//enemyForward.mMin.y = enemyBox.mMin.y;
	//enemyForward.mMin.z = enemyBox.mMin.z + 100;
	//enemyForward.mMax.x = enemyForward.mMin.x + 100.0f;
	//enemyForward.mMax.y = enemyForward.mMin.y + 100.0f;
	//enemyForward.mMax.z = enemyForward.mMin.z + 100.0f;
	//SetTriggerBox(EnemyTriggerEnum::ForwardBox, enemyForward);

}

NearEnemy::~NearEnemy()
{
	std::cout << "NearEnemy�j��" << mID << this << std::endl;
}

void NearEnemy::UpdateActor(float deltaTime)
{
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
	}*/
}

void NearEnemy::OnCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	//�����蔻��œG�Ɣw�i�Ƃ̏Փ˂̏ꍇ
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
	if (mHitBox == hitThisBox &&
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

void NearEnemy::HitAttackPlayer(BoxCollider* playerBox, BoxCollider* enemyBox)
{
	//�G�ɍU�������������Ƃ�
	if (playerBox->GetType() == EnumPhysicsType::EnumPlayer &&
		enemyBox->GetType() == EnumPhysicsType::EnumEnemyAttack)
	{
		//���������Ƃ��ɂ�������
		RemoveEnemyAttackHitBox();
	}
}

void NearEnemy::SetEnemyAttackHitBox()
{
	//�v���C���[�U���p�{�b�N�X�ݒu
	AABB EnemyAttackBox;

	mEnemyAttackBox = new BoxCollider(this, EnumPhysicsType::EnumEnemyAttack);

	EnemyAttackBox.mMin.x = 10;
	EnemyAttackBox.mMin.y = -50;
	EnemyAttackBox.mMin.z = 100;

	EnemyAttackBox.mMax.x = EnemyAttackBox.mMin.x + 150.0f;
	EnemyAttackBox.mMax.y = EnemyAttackBox.mMin.y + 150.0f;
	EnemyAttackBox.mMax.z = EnemyAttackBox.mMin.z + 150.0f;
	mEnemyAttackBox->SetObjectBox(EnemyAttackBox);
	mEnemyAttackBox->OnUpdateWorldTransform();
}

void NearEnemy::RemoveEnemyAttackHitBox()
{
	//�ߐړG�̍U���{�b�N�X���G�ɓ���������
	if (mEnemyAttackBox)
	{
		//�U���̃{�b�N�X������
		delete mEnemyAttackBox;
		mEnemyAttackBox = nullptr;
	}
}

bool NearEnemy::IsFrontHit()
{
	return mFrontTriggerBox->IsTrigerHit();
}
