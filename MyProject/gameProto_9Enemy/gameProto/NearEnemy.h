#pragma once
#include "EnemyActorBase.h"
#include "BoxCollider.h"

class  NearEnemy : public EnemyActorBase
{
public:
	 NearEnemy();
	~ NearEnemy();

	void UpdateActor(float deltaTime)override;
	void OnCollision(BoxCollider* hitThisBox, class BoxCollider* hitOtherBox) override;

	bool IsFrontHit();

	//�v���C���[�Ƃ̓����蔻��
	void						HitAttackPlayer(class BoxCollider* playerBox, class BoxCollider* enemyBox);
	void						SetEnemyAttackHitBox();		//�����蔻��̕t�^
	void						RemoveEnemyAttackHitBox();	//�����蔻��̏���

private:

	class BoxCollider* mHitBox;
	class BoxCollider* mHitGroundBox;	
	class EnemyBehaviorComponent* mEnemyBehaviorComponent;
	class BoxCollider* mFrontTriggerBox;
	//�U���p�����蔻��{�b�N�X
	class BoxCollider* mEnemyAttackBox;	   // �v���C���[�̍U�������蔻��{�b�N�X
};