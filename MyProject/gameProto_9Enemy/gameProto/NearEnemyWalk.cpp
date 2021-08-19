#include "EnemyActorBase.h"
#include "NearEnemyWalk.h"

NearEnemyWalk::NearEnemyWalk(EnemyBehaviorComponent* owner)
	:EnemyState(owner)
	,mTurnMode(false)
	,mAimDir(0.0f, 0.0f, 0.0f)
	,mAngleSpeed(0.0f)
	,mTurnNum(0)
{
	mStateType = EnemyStateEnum::Walk;
	mWalkSpeed = mOwnerActor->GetWalkSpeed();
	mAngleSpeed = mOwnerActor->GetTurnSpeed();
	mWalkRemainTime = mWalkTime;

	// パトロール用の現在向いてるときと逆向き方向の取得
	mAimDir = mOwnerActor->GetForward();
	mPrevAimDir = -1.0f * mAimDir;
}

NearEnemyWalk::~NearEnemyWalk()
{
}

EnemyStateEnum NearEnemyWalk::Update(float deltaTime)
{
	// Actorの現在位置・向きを取得
	Vector3 pos, forward;
	pos = mOwnerActor->GetPosition();
	forward = mOwnerActor->GetForward();

	// ターン時行動
	if (mTurnMode)
	{
		// 前進方向をmAimDirに徐々に向ける
		Matrix4 rot = Matrix4::CreateRotationZ(mAngleSpeed * deltaTime);
		forward = Vector3::Transform(forward, rot);
		forward.Normalize();

		// 許容目標角度になったらmAimDirにしTurnmodeを解除
		const float epsiron = 0.03f;
		float cos = Vector3::Dot(forward, mAimDir);
		if (fabs(1.0f - cos) < epsiron)
		{
			forward = mAimDir;
			mTurnMode = false;
			mWalkRemainTime = mWalkTime;
			mTurnNum++;

			//規定回数振り向いたら見渡し行動に移行
			if (mTurnNum >= mTurnMaxNum)
			{
				// 見渡し行動に移行
				mOwnerActor->SetForwardVec(forward);
				mOwnerActor->SetComputeWorldTransform();
				return EnemyStateEnum::Idle;
			}
		}
	}
	else
	{
		// 前進行動の残り時間算出し、経過したら向きを変更
		mWalkRemainTime -= deltaTime;
		if (mWalkRemainTime < 0.0f)
		{
			//進行方向と逆方向を目標向きに変更
			Vector3 tmpDir = mAimDir;
			mAimDir = mPrevAimDir;
			mPrevAimDir = tmpDir;
			mTurnMode = true;

			return mStateType;
		}
		// 位置・前進方向より、次の位置を算出
		pos += forward * mWalkSpeed * deltaTime;
	}
	// Actorの位置・向きの更新
	mOwnerActor->SetForwardVec(forward);
	mOwnerActor->SetPosition(pos);
	mOwnerActor->SetComputeWorldTransform();

	//追跡にstateを移動
	//Vector3でエネミーとプレイヤーの位置を取る
	//エネミーの場所　プレイヤーの場所　2点間の距離
	Vector3 enemyPos, playerPos;

	//エネミーの位置を取得
	enemyPos = mOwnerActor->GetPosition();

	//プレイヤーの位置を取得
	playerPos = GAMEINSTANCE.GetPlayerPos();

	//敵キャラからプレイヤーに向かうベクトルを計算
	Vector3 distanceVec = playerPos - enemyPos;
	float distance = distanceVec.Length();


	////位置によってステートを変える
	//if (distance <= 1300)
	//{
	//	return EnemyStateEnum::Chase;
	//}

	// 歩き行動継続
	return EnemyStateEnum::Walk;
}

void NearEnemyWalk::OnEnter()
{
	// 歩きアニメーションスタート
	mOwnerActor->PlayAnimation(EnemyStateEnum::Walk, 0.5f);
	mTurnNum = 0;
}

void NearEnemyWalk::OnExit()
{
}
