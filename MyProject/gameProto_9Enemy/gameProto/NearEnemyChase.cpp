#include "NearEnemyChase.h"
#include "EnemyActorBase.h"

NearEnemyChase::NearEnemyChase(EnemyBehaviorComponent* owner)
    :EnemyState(owner)
{
    //�G�l�~�[�X�e�[�g��ǐՂɂ���
    mStateType = EnemyStateEnum::Chase;
    //�X�s�[�h���擾
    mWalkSpeed = mOwnerActor->GetWalkSpeed();

    printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

NearEnemyChase::~NearEnemyChase()
{
    printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum NearEnemyChase::Update(float deltaTime)
{
    //Vector3�ŃG�l�~�[�ƃv���C���[�̈ʒu�����
   //�G�l�~�[�̏ꏊ�@�v���C���[�̏ꏊ�@2�_�Ԃ̋���
    Vector3 enemyPos, playerPos, epDir;

    //�G�l�~�[�̈ʒu���擾
    enemyPos = mOwnerActor->GetPosition();

    //�v���C���[�̈ʒu���擾
    playerPos = GAMEINSTANCE.GetPlayerPos();

    //�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z
    epDir = playerPos - enemyPos;
    epDir.Normalize();

    //�G�L�����̌����������Z�b�g
    //�G�̎������v���C���[����
    mOwnerActor->SetForwardVec(epDir);

    //�����x�N�g�����擾
    Vector3 enemyForward = mOwnerActor->GetForward();
    enemyForward.Normalize();

    //�G�L��������v���C���[�Ɍ������x�N�g�����v�Z
    Vector3 distanceVec = playerPos - enemyPos;
    float distance = distanceVec.Length();

    //�G�̎��������x�N�g���ƓG����v���C���[�Ɍ������x�N�g���̊Ԃ�cos���v�Z
    //�x�N�g���v�Z
    float cos = Vector3::Dot(enemyForward, epDir);

    //�G�l�~�[���|��Ȃ��悤�ɂ���(z���������I��0�ɂ���)
    enemyForward.z = 0.0f;

    //�G�l�~�[�̑O�ɃX�s�[�h���������l����
    enemyPos += enemyForward * mWalkSpeed * deltaTime;

    //�G�l�~�[�𓮂���
    mOwnerActor->SetPosition(enemyPos);

    //�ʒu�ɂ���ăX�e�[�g��ς���

    if (distance <= 1000)
    {
        return EnemyStateEnum::Attack1;
    }

    return EnemyStateEnum::Chase;
}

void NearEnemyChase::OnEnter()
{
    // �����A�j���[�V�����X�^�[�g
    mOwnerActor->PlayAnimation(EnemyStateEnum::Chase, 0.5f);
}

void NearEnemyChase::OnExit()
{
}
