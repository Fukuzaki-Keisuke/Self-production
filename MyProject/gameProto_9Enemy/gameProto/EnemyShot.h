#pragma once
#include "EnemyActorBase.h"
#include "EnemyBehaviorComponent.h"

class EnemyShot : public Actor
{
public:
	EnemyShot();
	~EnemyShot();
	void                         UpdateActor(float deltaTime) override;
	void                         FixShotCollision(class BoxCollider* hitThisBox, class BoxCollider* hiOtherBox);
	void						 HitShot();
	/*void						 ShotFront(Vector3 pos);*/
	void						 SetShot(Vector3 pos);
	/*void						 setPPos(Vector3 pos);*/

	//�v���C���[�Ƃ̓����蔻��
	void						 HitPlayer(class BoxCollider* playerBox, class BoxCollider* mEnemyShotBox);
	//void						 SetAttackBulletBox();		//�����蔻��̕t�^
	void						 RemoveAttackBulletBox();	//�����蔻��̏���

	//�v���C���[�̍U���ƓG�̒e�Ƃ̓����蔻��
	void						 EnemyBBandPlayerAB(class BoxCollider* mEnemyShotBox,class BoxCollider* mPlayerAttackBox);

private:
	class MeshComponent* mMeshComp;
	class BoxCollider* mShotBox;
	class BlackKnightActor* Position;

	//�U���p�����蔻��{�b�N�X
	class BoxCollider* mAttackBox; // �v���C���[�̍U�������蔻��{�b�N�X

	//�p�x
	float mAngle;
	//�e�̈ʒu
	Vector3 mshotpos;

	Vector3 shotPos;
	bool TimerFlag;
	Vector3 mFront;
	bool	shotFlag;
	Vector3 mPPos;
};