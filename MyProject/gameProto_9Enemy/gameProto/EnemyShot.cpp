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
	//メッシュのロード
	Mesh* mesh = RENDERER->GetMesh("Assets/Enemy/EnemyBullet.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	//エネミーの弾の当たり判定
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
	//アクターの前進ベクトルを取る
	Vector3 shotForwardVec = GetForward();

	//最初の弾の位置
	Vector3 shotpos = GetPosition();
	
	//弾のスピード
	const float speed = 350.0f;

	//アクターのポジションに足していく
	mPosition += deltaTime * speed * shotForwardVec;
	 
	//ワールドの再計算
	mRecomputeWorldTransform = true; 

	//一定以上離れたら消す
}

//壁との当たり判定(マップ追加次第)
void EnemyShot::FixShotCollision(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	
	Vector3 fix;
	Vector3 vex(0, -5, 0);

	//壁とぶつかったとき
	AABB bgBox = hitOtherBox->GetWorldBox();
	AABB shotBox = mShotBox->GetWorldBox();

	//敵の弾自体を削除
	//デストラクタを呼ぶ
	HitShot();
}

//弾を消す処理
void EnemyShot::HitShot()
{
	SetState(Actor::EDead);
}

//弾の補正関係
void EnemyShot::SetShot(Vector3 pos)
{
	//弾の補正(無いと埋まる)
	shotPos = pos;
	Vector3 cor(1, 1, 70);
	mPosition = shotPos + cor;
}

//敵の弾とプレイヤーとの当たり判定
void EnemyShot::HitPlayer(BoxCollider* hitThisBox, BoxCollider* hitOtherBox)
{
	if (mAttackBox == hitThisBox &&
		hitOtherBox->GetType() == EnumPhysicsType::EnumPlayer)
	{
		//当たったときにすぐ消す
		RemoveAttackBulletBox();
	}
}

//敵の弾ボックスの消去
void EnemyShot::RemoveAttackBulletBox()
{
	if (mAttackBox)
	{
		//敵の弾の当たり判定の削除
		delete mAttackBox;
		mAttackBox = nullptr;

		//敵の弾自体を削除
		//デストラクタを呼ぶ
		HitShot();
	}
}

//敵の弾とプレイヤーの攻撃の当たり判定ボックス
void EnemyShot::EnemyBBandPlayerAB(BoxCollider* mPlayerAttackBox, BoxCollider* mEnemyShotBox)
{
	if (mAttackBox == mPlayerAttackBox &&
		mEnemyShotBox->GetType() == EnumPhysicsType::EnumPlayerAttack)
	{
		printf("敵の弾とプレイヤーの攻撃ボックスが当たってるよ\n");

		//当たったときにすぐ消す
		RemoveAttackBulletBox();
	}
}