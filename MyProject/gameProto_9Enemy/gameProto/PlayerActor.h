#pragma once
#include "Actor.h"
#include "Collision.h"

enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,     // �ҋ@
	PLAYER_STATE_RUN,          // ����
	PLAYER_STATE_JUMPSTART,    // �W�����v�X�^�[�g
	PLAYER_STATE_JUMPLOOP,     // �W�����v���[�v
	PLAYER_STATE_JUMPEND,      // �W�����v�I��
	PLAYER_STATE_LIGHTATTACK1, // ��U��1
	PLAYER_STATE_LIGHTATTACK2, // ��U��2
	PLAYER_STATE_LIGHTATTACK3, // ��U��3
	PLAYER_STATE_HIT,		   // �_���[�W

	PLAYER_STATE_NUM,   // ���A�j���[�V������
};

// ���@�g���K�[�{�b�N�X���
enum class PlayerTriggerEnum : unsigned char
{
	ForwardBox,          // ����
	LeftBox,             // ����
	RightBox,            // �E
	BackBox,             // ���
	HeadBox,             // ����
	GroundBox,           // ����
	GroundForwardBox     // �����O��
};


class PlayerActor : public Actor
{
public:
	PlayerActor();
	~PlayerActor();

	void UpdateActor(float deltaTime) override;
	void									 FixCollision(class BoxCollider *hitPlayerBox, class BoxCollider *hitBox);
	void									 FixEnemyCollision(class BoxCollider* hitPlayerBox, class BoxCollider* enemyBox);
	class SkeletalMeshComponent*			 GetSkeletalMeshComp();
	const class Animation*					 GetAnim(PlayerState state);
	bool									 GetIsJump()const { return mIsJump; }
	bool									 GetOnground() const { return mOnGroundChk; }
	Vector3&								 GetJumpVec() { return mJumpVec; }

	void									 SetOnground(bool onGround) { mOnGroundChk = onGround; }
	void									 SetIsJump(bool jump) { mIsJump = jump; }
	void									 SetJumpVec(Vector3& vec) { mJumpVec = vec; }

	//�̗�
	void									 SetmPHitPoint(int hitpoint) { mPHitPoint = hitpoint; }  // �q�b�g�|�C���g�Z�b�g
	int										 GetmPHitPoint() const { return mPHitPoint; }  // �̗͂̎擾


	//�G�Ƃ̓����蔻��֌W
	void									 HitEnemy(class BoxCollider* playerBox, class BoxCollider* enemyBox); //�ΏۂƂ̓����蔻��
	void									 SetAttackHitBox();		//�����蔻��̕t�^
	void									 RemoveAttackHitBox();	//�����蔻��̏���

	//�G�̋ߐڍU���Ƃ̓����蔻��
	void									 HitEnemyAttackDamage(class BoxCollider* hitPlayerBox, class BoxCollider* mEnemyAttackBox);

	//�G�̒e�Ƃ̓����蔻��
	void									 HitBullet(class BoxCollider* hitPlayerBox, class BoxCollider* mEnemyShotBox);	//�ΏۂƂ̓����蔻��̕t�^

	//�G�̒e�ƍU���{�b�N�X�̓����蔻��
	void									 PlayerABandEnemyBB(class BoxCollider* mPlayerAttackBox, class BoxCollider* mEnemyShotBox);


private:
	class SkeletalMeshComponent*             mMeshComp;
	std::vector<const class Animation*>      mAnimTypes;
	class BoxCollider*                       mHitBox;
	class BoxCollider*                       mHitGroundBox;
	class BoxCollider*                       mHitHeadBox;

	//�U���p�����蔻��{�b�N�X
	class BoxCollider*						 mAttackBox;	   // �v���C���[�̍U�������蔻��{�b�N�X

	PlayerState                              mNowState;        // ���݂̃X�e�[�g
	PlayerState                              mNextState;       // ���̃X�e�[�g
	std::vector<class PlayerActorStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��

	int										 mPHitPoint;	   // �̗�

	bool                                     mIsJump;
	bool                                     mOnGroundChk;
	Vector3                                  mJumpVec;
	Vector3                                  mVelocityVec;
};