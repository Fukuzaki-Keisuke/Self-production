#include "EnemyChase.h"
#include "EnemyActorBase.h"
#include "Game.h"
#include "EnemyPatrol.h"

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner)
    :EnemyState(owner)
{
    //エネミーステートを追跡にする
    mStateType = EnemyStateEnum::Chase;
    //スピードを取得
    mWalkSpeed = mOwnerActor->GetWalkSpeed();

    printf("Create : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyChase::~EnemyChase()
{
    printf("Remove : [EnemyState] EnemyHit (0x%p)\n", mOwnerActor);
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
    //Vector3でエネミーとプレイヤーの位置を取る
    //エネミーの場所　プレイヤーの場所　2点間の距離
    Vector3 enemyPos, playerPos, epDir;

    //エネミーの位置を取得
    enemyPos = mOwnerActor->GetPosition();

    //プレイヤーの位置を取得
    playerPos = GAMEINSTANCE.GetPlayerPos();

    //エネミーからプレイヤーに向かうベクトルを計算
    epDir = playerPos - enemyPos;
    epDir.Normalize();
    
    //敵キャラの向き方向をセット
    //敵の視線がプレイヤー向く
    mOwnerActor->SetForwardVec(epDir);

    //向きベクトルを取得
    Vector3 enemyForward = mOwnerActor->GetForward();
    enemyForward.Normalize();

    //敵キャラからプレイヤーに向かうベクトルを計算
    Vector3 distanceVec = playerPos - enemyPos;
    float distance = distanceVec.Length();

    //敵の視線向きベクトルと敵からプレイヤーに向かうベクトルの間のcosを計算
    //ベクトル計算
    float cos = Vector3::Dot(enemyForward, epDir);

    //エネミーが倒れないようにする(z軸を強制的に0にする)
    enemyForward.z = 0.0f;

    //エネミーの前にスピードをかけた値を代入
    enemyPos += enemyForward * mWalkSpeed * deltaTime;

    //エネミーを動かす
   /* mOwnerActor->SetPosition(enemyPos);*/

    //位置によってステートを変える

    if (distance <= 800)
    {
        return EnemyStateEnum::Attack1;
    }

    return EnemyStateEnum::Chase;

}

void EnemyChase::OnEnter()
{
    // 歩きアニメーションスタート
    mOwnerActor->PlayAnimation(EnemyStateEnum::Walk, 0.5f);
}

void EnemyChase::OnExit()
{
}
