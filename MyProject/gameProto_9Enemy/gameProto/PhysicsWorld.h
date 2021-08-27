#pragma once
#include "Game.h"
#include "BoxCollider.h"

enum class EnumPhysicsType
{
	EnumEnemy,                // �G�l�~�[
	EnumPlayer,				  // �v���[���[
	EnumBG,					  // �w�i�u���b�N
	EnumCoin,				  // �R�C��
	EnumHitCheck,		      // �q�b�g�`�F�b�N�p�i�����Ԃ��Ȃ�)
	EnumBGTrigger,            // �w�i�ƐڐG�������̃g���K�[
	EnumPlayerAttack,		  // �v���C���[�U������
	EnumEnemyAttack,		  // �G�l�~�[�U���ǌ|
	EnumEnemyShot			  // �G�̒e�̓����蔻��
};

class PhysicsWorld
{
public:

	PhysicsWorld();
	~PhysicsWorld();
	void AddBoxCollider(EnumPhysicsType type, class BoxCollider* box); // BoxCollider(AABB)�ǉ�
	void RemoveBoxCollider(class BoxCollider* box);                    // BoxCollider�폜
	void DebugShowBoxLists();                                          // �{�b�N�X���X�g�\���i�f�o�b�O�p)
	void Collision();                                                  // �R���W����
	void DebugShowBox();                                               // �f�o�b�O�p�{�b�N�X�\��
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }       // �f�o�b�O���[�h

private:
	bool                              mBoolDebugMode ;                 // �f�o�b�O���[�h
	std::vector<class BoxCollider*>   mBGBoxs        ;                 // �w�i������f�[�^
	std::vector<class BoxCollider*>   mPlayerAttackBox;				   //�v���C���[�̍U��������f�[�^
	std::vector<class BoxCollider*>   mPlayerBoxs    ;                 // �v���[���[������f�[�^
	std::vector<class BoxCollider*>   mCoins         ;                 // �R�C��������f�[�^

	std::vector<class BoxCollider*>   mEnemies;					       // �G�L����             
	std::vector<class BoxCollider*>   mBGTriggers;                     // �w�i�Ƃ̐ڐG����p�g���K�[

	std::vector<class BoxCollider*>   mEnemyAttackBox;				   //�v���C���[�̍U��������f�[�^
	std::vector<class BoxCollider*>   mEnemyShotBox;				   //�G�e�̂�����f�[�^

	void PlayerAndBGTest();  // �v���[���[�ƕǂƂ̂����蔻��
	void EnemyAndBGTest();   // �G�ƕǂƂ̓����蔻��
	void TriggerAndBGTest(); // �w�i�g���K�[�Ɣw�i�̔���e�X�g
	void InitBoxVertices();
	void PlayerAttackAndEnemyTest();	//�v���C���[�̍U���{�b�N�X�ƃG�l�~�[�̓����蔻��
	void EnemyAttackBoxAndPlayerTest();    //�G�l�~�[�̍U���{�b�N�X�ƃv���C���[�̓����蔻��
	void EnemyAttackAndPlayerTest();	//�G�e�ƃv���C���[�̓����蔻��
	void PlayerABandEnemyBBTest();		//�v���C���[�̍U���{�b�N�X�ƓG�̒e�̓����蔻��
	void EnemyBBTestandBGTest();		//�G�̒e�ƕǂƂ̓����蔻��
	void DrawBoxs(std::vector<class BoxCollider*>& boxs, const Vector3& color);
	unsigned int mBoxVAO; // �{�b�N�X�`��p��VAO  
	class Shader* mLineShader; // ���C���`��p�V�F�[�_�[
};