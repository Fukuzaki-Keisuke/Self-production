#include "EnemyShot.h"

#include "game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Collision.h"
#include "BoxCollider.h"

EnemyShot::EnemyShot()
	: mAngle(0.0f)
	, shotFlag(false)
{
	//���b�V���̃��[�h
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/EnemyBullet.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	//�G�l�~�[�̒e�̓����蔻��
	AABB shotBox = mesh->GetCollisionBox();
	shotBox.mMin.x *= 1.0f;
	shotBox.mMax.x *= 1.0f;
	mAttackBox = new BoxCollider(this, EnumPhysicsType::EnumEnemyShot);
	mAttackBox->SetObjectBox(shotBox);
	mAttackBox->OnUpdateWorldTransform();
}

EnemyShot::~EnemyShot()
{
}

void EnemyShot::UpdateActor(float deltaTime)
{
	//�A�N�^�[�̑O�i�x�N�g�������
	Vector3 shotForwardVec = GetForward();

	//�ŏ��̒e�̈ʒu
	Vector3 shotpos = GetPosition();
	
	//�e�̃X�s�[�h
	const float speed = 350.0f;

	//�A�N�^�[�̃|�W�V�����ɑ����Ă���
	mPosition += deltaTime * speed * shotForwardVec;
	 
	//���[���h�̍Čv�Z
	mRecomputeWorldTransform = true; 

	//���ȏ㗣�ꂽ�����
}

//�ǂƂ̓����蔻��(�}�b�v�ǉ�����)
void EnemyShot::FixShotCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	
	Vector3 fix;
	Vector3 vex(0, -5, 0);

	//�ǂƂԂ������Ƃ�
	AABB bgBox = hitOtherBox->GetWorldBox();
	AABB shotBox = mShotBox->GetWorldBox();

	//�G�̒e���̂��폜
	//�f�X�g���N�^���Ă�
	HitShot();
}

//�e����������
void EnemyShot::HitShot()
{
	SetState(Actor::EDead);
}

//�e�̕␳�֌W
void EnemyShot::SetShot(Vector3 pos)
{
	//�e�̕␳(�����Ɩ��܂�)
	shotPos = pos;
	Vector3 cor(1, 1, 70);
	mPosition = shotPos + cor;
}

//�G�̒e�ƃv���C���[�Ƃ̓����蔻��
void EnemyShot::HitPlayer(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	if (mAttackBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayer)
	{
		//���������Ƃ��ɂ�������
		RemoveAttackBulletBox();
	}
}

//�G�̒e�{�b�N�X�̏���
void EnemyShot::RemoveAttackBulletBox()
{
	if (mAttackBox)
	{
		//�G�̒e�̓����蔻��̍폜
		delete mAttackBox;
		mAttackBox = nullptr;

		//�G�̒e���̂��폜
		//�f�X�g���N�^���Ă�
		HitShot();
	}
}

//�G�̒e�ƃv���C���[�̍U���̓����蔻��{�b�N�X
void EnemyShot::EnemyBBandPlayerAB(BoxCollider* mPlayerAttackBox, BoxCollider* mEnemyShotBox)
{
	if (mAttackBox == mPlayerAttackBox &&
		mEnemyShotBox->GetType() == EnumPhysicsType::EnumPlayerAttack)
	{
		printf("�G�̒e�ƃv���C���[�̍U���{�b�N�X���������Ă��\n");

		//���������Ƃ��ɂ�������
		RemoveAttackBulletBox();
	}
}