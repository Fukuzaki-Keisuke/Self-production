#include "PlayerActor.h"
#include "Game.h"
#include "Input.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "AudioManager.h"

#include "PlayerActorStateBase.h"
#include "PlayerActorStateRun.h"
#include "PlayerActorStateIdle.h"
#include "PlayerActorStateJump.h"
#include "PlayerActorStateJumpLoop.h"
#include "PlayerActorStateJumpEnd.h"
#include "PlayerActorStateLightAttack1.h"
#include "PlayerActorStateLightAttack2.h"
#include "PlayerActorStateLightAttack3.h"
#include "PlayerActorStateHIt.h"

#include "EnemyBehaviorComponent.h"
#include "EnemyPatrol.h"
#include "EnemyLookAround.h"
#include "EnemyHit.h"
#include "EnemyShot.h"

#include <iostream>

const float cAnimationSpeed = 0.5f;

PlayerActor::PlayerActor()
	: mNowState(PlayerState::PLAYER_STATE_IDLE)
	, mNextState(PlayerState::PLAYER_STATE_IDLE)
	, mIsJump(false)
	, mOnGroundChk(true)
	, mPHitPoint(10)		//�̗�
{

	//���b�V���̃��[�h
	Mesh* mesh = RENDERER->GetMesh("Assets/Player/Player_Idle.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// �X�P���g��
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Player/Player_Idle.gpskel"));
	
	// �A�j���[�V�����̎擾 & �A�j���[�V�����z��ɃZ�b�g
	mAnimTypes.resize(static_cast<unsigned int>(PlayerState::PLAYER_STATE_NUM));
	//�A�C�h��
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)]      = RENDERER->GetAnimation("Assets/Player/Player_Idle_Anim.gpanim", true);
	//����
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_RUN)]       = RENDERER->GetAnimation("Assets/Player/Player_Running.gpanim", true);
	//�W�����v�X�^�[�g
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPSTART)] = RENDERER->GetAnimation("Assets/Player/Jumping_Up.gpanim", false);
	//�W�����v���[�v
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPLOOP)]  = RENDERER->GetAnimation("Assets/Player/Player_Falling.gpanim", false);
	//�W�����v�I���
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_JUMPEND)]	 = RENDERER->GetAnimation("Assets/Player/Jumping_Down.gpanim", false);
	//��U��1
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_LIGHTATTACK1)] = RENDERER->GetAnimation("Assets/Player/Player_Attack1.gpanim", false);
	//��U��2
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_LIGHTATTACK2)] = RENDERER->GetAnimation("Assets/Player/Player_Attack2.gpanim", false);
	//��U��3
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_LIGHTATTACK3)] = RENDERER->GetAnimation("Assets/Player/Player_Attack3.gpanim", false);
	//�_���[�W
	mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_HIT)] = RENDERER->GetAnimation("Assets/Player/Player_Hit.gpanim", false);


	// �A�C�h����ԃA�j���[�V�������Z�b�g
	mMeshComp->PlayAnimation( mAnimTypes[static_cast<unsigned int>(PlayerState::PLAYER_STATE_IDLE)], cAnimationSpeed);

	// �A�N�^�[�X�e�[�g�v�[���̏�����
	mStatePools.push_back(new PlayerActorStateIdle);          // mStatePool[PLAYER_STATE_IDLE]
	mStatePools.push_back(new PlayerActorStateRun);           // mStatepool[PLAYER_STATE_RUN]
	mStatePools.push_back(new PlayerActorStateJump);          // mStatepool[PLAYER_STATE_JUMPSTART];  
	mStatePools.push_back(new PlayerActorStateJumpLoop);      // mStatepool[PLAYER_STATE_JUMPLOOP];
	mStatePools.push_back(new PlayerActorStateJumpEnd);       // mStatepool[PLAYER_STATE_JUMPEND];
	mStatePools.push_back(new PlayerActorStateLightAttack1);  // mStatepool[PLAYER_STATE_LightAttack1];
	mStatePools.push_back(new PlayerActorStateLightAttack2);  // mStatepool[PLAYER_STATE_LightAttack2];
	mStatePools.push_back(new PlayerActorStateLightAttack3);  // mStatepool[PLAYER_STATE_LightAttack3];
	mStatePools.push_back(new PlayerActorStateHIt);			  // mStatepool[PLAYER_STATE_HIT];

	// �����蔻��Z�b�g
	AABB playerBox = mesh->GetCollisionBox();
	mHitBox = new BoxCollider(this,EnumPhysicsType::EnumPlayer);
	playerBox.mMin.x *= 0.8f;
	playerBox.mMin.y *= 0.8f;
	playerBox.mMax.x *= 0.8f;
	playerBox.mMax.y *= 0.8f;
	mHitBox->SetObjectBox(playerBox);

	// �v���[���[�̑����𒲂ׂ�{�b�N�X���쐬�@�{�b�N�X��1/4, �{�b�N�X��ʂ����_�ɗ���悤�ɂ���
	AABB groundBox;
	groundBox = playerBox;
	groundBox.mMin.x *= 0.8f;
	groundBox.mMin.y *= 0.8f;
	groundBox.mMax.x *= 0.8f;
	groundBox.mMax.y *= 0.8f;
	groundBox.mMin.z = -2.0f;  //�W�����v���Ɉ���������Ȃ�����
	groundBox.mMax.z *= 0.0f;
	mHitGroundBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitGroundBox->SetObjectBox(groundBox);

	// �v���[���[�̓���𒲂ׂ�{�b�N�X���쐬 �{�b�N�X��ʂ�����ɗ���悤�ɂ���
	AABB headBox;
	headBox = groundBox;
	headBox.mMin.z = playerBox.mMax.z;
	headBox.mMax.z = headBox.mMin.z + 2.0f;
	mHitHeadBox = new BoxCollider(this, EnumPhysicsType::EnumPlayer);
	mHitHeadBox->SetObjectBox(headBox);
	
	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", mID, this);
	mJumpVec = Vector3(0, 0, 0);

	//�v���C���[�̈ʒu���擾
	GAMEINSTANCE.SetPlayerActor(this);
}

PlayerActor::~PlayerActor()
{
	//�o�^�X�e�[�g�����ׂĉ��
	for (auto ps : mStatePools)
	{
		delete ps;
	}
	mAnimTypes.clear(); //�A�j���[�V�����{�̂̏����̓����_���[���ōs����
	printf("PlayerActor�j�� id:[%5d] this : (0x%p)\n", mID, this);
}

void PlayerActor::UpdateActor(float deltaTime)
{
	// �O�̃V�[�����珰���Ȃ�������
	if (!mIsJump && !mOnGroundChk)
	{
		mIsJump = true;
		mNextState = PlayerState::PLAYER_STATE_JUMPLOOP;
	}
	mOnGroundChk = false;

	// �X�e�[�g�O������X�e�[�g�ύX�����������H
	if (mNowState != mNextState)
	{
		//���̃X�e�[�g��Exit�ɓ���
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		//���̃X�e�[�g��Enter�ɐi��
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// �X�e�[�g���s
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// �X�e�[�g��������X�e�[�g�ύX���������H
	if (mNowState != mNextState)
	{
		//���̃X�e�[�g��Exit�ɓ���
		mStatePools[static_cast<unsigned int>(mNowState)]->Exit(this, deltaTime);
		//���̃X�e�[�g��Enter�ɐi��
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	//����̓����蔻��̍X�V����

}


// �w�iAABB�Ƃ̃q�b�g�߂荞�݉��� ( ���������ۂ�PhysicsWorld����Ă΂�� �j
void PlayerActor::FixCollision(BoxCollider *hitPlayerBox, BoxCollider * hitBox)
{

	Vector3 fix;
	// �W�����v�I����
	if (hitPlayerBox == mHitGroundBox && mJumpVec.z <= 0.0f)
	{
		// �n�ʔ���p�{�b�N�X���n�ʂƓ������Ă���(�ڒn�`�F�b�N�j
		mOnGroundChk = true;

		// �W�����v���Ȃ����
		if (mIsJump)
		{
			mOnGroundChk = true;
			mIsJump = false;
			mJumpVec = Vector3(0, 0, -1.0f);
			mPosition += mJumpVec;
			mNextState = PlayerState::PLAYER_STATE_JUMPEND;
			return;
		}
	}

	//�ǂƂԂ������Ƃ�
	AABB bgBox = hitBox->GetWorldBox();
	AABB playerBox = mHitBox->GetWorldBox();

	// �߂荞�݂��C��
	calcCollisionFixVec(playerBox, bgBox, fix);

	// �␳�x�N�g�����߂�
	mPosition += fix;

	// �W�����v�A�N�V�������ɓ����ǂɓ��������Ƃ�
	if (hitPlayerBox == mHitHeadBox && mIsJump)
	{
		//�W�����v������ɍs���������t�]������
		mJumpVec = Vector3(0, 0, -1.0f);
		mPosition += mJumpVec; // ��������ɓ�����Ȃ��l�ɗ���
	}

	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	mHitBox->OnUpdateWorldTransform();

}

//�G�Ƃ̓����蔻��
void PlayerActor::FixEnemyCollision(BoxCollider* hitPlayerBox, BoxCollider* enemyBox)
{
	if (hitPlayerBox != mHitBox)
	{
		return;
	}
	Vector3 fix;

	//�ǂƂԂ������Ƃ�
	AABB EBox = enemyBox->GetWorldBox();
	AABB playerBox = hitPlayerBox->GetWorldBox();

	//�߂荞�ݏC��
	calcCollisionFixVec(playerBox,EBox, fix);

	//�␳�x�N�g�����߂�
	mPosition += fix;

	//�ʒu���ς�����̂Ń{�b�N�X���Čv�Z
	hitPlayerBox->OnUpdateWorldTransform();
	mRecomputeWorldTransform = true;
}

SkeletalMeshComponent* PlayerActor::GetSkeletalMeshComp()
{
	return mMeshComp;
}

const Animation* PlayerActor::GetAnim(PlayerState state)
{
	return mAnimTypes[static_cast<unsigned int>(state)];
}

void PlayerActor::SetAttackHitBox()
{
	//�v���C���[�U���p�{�b�N�X�ݒu
	AABB playerAttackBox;

	mAttackBox = new BoxCollider(this, EnumPhysicsType::EnumPlayerAttack);

	playerAttackBox.mMin.x = 10;
	playerAttackBox.mMin.y = -50;
	playerAttackBox.mMin.z = 100;

	playerAttackBox.mMax.x = playerAttackBox.mMin.x + 150.0f;
	playerAttackBox.mMax.y = playerAttackBox.mMin.y + 150.0f;
	playerAttackBox.mMax.z = playerAttackBox.mMin.z + 150.0f;
	mAttackBox->SetObjectBox(playerAttackBox);
	mAttackBox->OnUpdateWorldTransform();
}

//�U���{�b�N�X�̏���
void PlayerActor::RemoveAttackHitBox()
{
	//�v���C���[�̍U���{�b�N�X���G�ɓ���������
	if (mAttackBox)
	{
		//�U���̃{�b�N�X������
		delete mAttackBox;
		mAttackBox = nullptr;
	}
}

//�G�Ƃ̓����蔻��
void PlayerActor::HitEnemy(class BoxCollider* playerBox, BoxCollider* enemyBox)
{
	//�G�ɍU�������������Ƃ�
	if (playerBox->GetType() == EnumPhysicsType::EnumPlayerAttack &&
		enemyBox->GetType() == EnumPhysicsType::EnumEnemy)
	{
		//���������Ƃ��ɂ�������
		RemoveAttackHitBox();
	}
}

//�G�Ƃ̓����蔻��(�_���[�W)
void PlayerActor::HitBullet(BoxCollider* hitPlayerBox, BoxCollider* mEnemyShotBox)
{
	//�G�̍U�������������Ƃ�
	if (mHitBox == hitPlayerBox &&
		mEnemyShotBox->GetType() == EnumPhysicsType::EnumEnemyShot)
	{
		printf("�G�̍U�����������Ă��\n");

		mPHitPoint -= 1;

		if (mNowState != PlayerState::PLAYER_STATE_HIT)
		{
			mNextState = PlayerState::PLAYER_STATE_HIT;
		}
	}

	//�󒆂œ��������Ƃ��ɕ����Ă��܂��̂�h��
	if (mIsJump)
	{
		mOnGroundChk = true;
		mIsJump = false;
		mJumpVec = Vector3(0, 0, -1.0f);
		mPosition += mJumpVec;
		return;
	}
}

//�G�̒e�ƃv���C���[�̍U���{�b�N�X�Ƃ̓����蔻��
void PlayerActor::PlayerABandEnemyBB(BoxCollider* mPlayerAttackBox, BoxCollider* mEnemyShotBox)
{
}