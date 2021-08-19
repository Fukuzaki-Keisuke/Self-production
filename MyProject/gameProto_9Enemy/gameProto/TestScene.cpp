#include "game.h"
#include "Math.h"
#include "Renderer.h"
#include "TestScene.h"
#include "PlayerActor.h"
#include "DebugGrid.h"
#include "LookDownCamera.h"
#include "ThirdPersonCamera.h"
#include "AudioManager.h"
#include "LevelManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "LevelActor.h"

#include "BlackKnightActor.h"
#include "NearEnemy.h"

#pragma warning(disable:4996)

TestScene::TestScene()
{ 
	//プレーヤーの初期位置
	mPlayer = new PlayerActor;
	mPlayer->SetPosition(Vector3(1000, -1800, 40));

	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.5f, 0.5f, 0.5f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection    = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor    = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid( 1000.0f, 30, color );

	//カメラ系
	ThirdPersonCamera* c = new ThirdPersonCamera(mPlayer);
	c->Init(Vector3(300, 0, 300), Vector3(0, 0, 10), Vector3(0, 0, 1));
	//カメラとの距離
	c->SetCameraLength(600.0f);

	AUDIO->GetSound("assets/audio/jump.wav");
	AUDIO->GetSound("assets/audio/coin.wav");
	AUDIO->GetMusic("assets/audio/loopMusic.wav");
	//AUDIO->PlayMusic("assets/audio/loopMusic.wav");

	//マップのgpmesh読み込み
	//レベル読み込み
	/*mLevel = new LevelManager;*/
	LevelActor* level = new LevelActor;
	Vector3 offs(-60, 380, 20);
	level->LoadLevel("assets/Map/MapStaticMesh.gpmesh", "assets/Map/collision.json", offs);

	//ヤドンのテクスチャ(使ってない)
	mTexture = RENDERER->GetTexture("assets/texture.png");
 	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "assets/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	//同じ敵を複数用意する
	//エネミーの初期位置

	//////雑魚エネミー1 左上
	//mEnemyBase = new BlackKnightActor;
	//mEnemyBase->SetPosition(Vector3(-800, 1300, 0));

	//雑魚エネミー2 右上
	mEnemyBase = new BlackKnightActor;
	mEnemyBase->SetPosition(Vector3(1000, 1300, 0));

	//近接のみのエネミー
	mEnemyBase = new NearEnemy;
	mEnemyBase->SetPosition(Vector3(-800, 1300, 40));
}

TestScene::~TestScene()
{
	delete mLevel;
}

SceneBase *TestScene::update()
{
	float radius = 160.0f;
	static float anim = 0.0f;
	anim += 0.01f;
	Matrix4 view;
	view = GAMEINSTANCE.GetViewMatrix();
	GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);

	//プレイヤーの位置を取る
	Vector3 playerPos;
	playerPos = GAMEINSTANCE.GetPlayerPos();

	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
 		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}

	//ゲームクリアシーンに移行

	//敵を全部倒したらクリア
	if (EnemyActorBase::GetEnemyCount() <= 0)
	{
		//時間の計測(staticは前のフレームの時間を保持する)
		static float time = 0.0f;
		//deltaTimeの取得
		float deltaTime = GAMEINSTANCE.GetDeltaTime();
		//timeにdeltaTimeを加算していく
		time = time + deltaTime;
		if (time > 3.0f)
		{
			GAMEINSTANCE.sceneShutdown();
			return new GameClearScene;
		}
	}

	//ゲームオーバーシーンに移行

	//プレイヤーの体力が0になると死亡
	if (mPlayer->GetmPHitPoint() <= 0)
	{
		//時間の計測
		static float time = 0.0f;
		//deltaTimeの取得(staticは前のフレームの時間を保持する)
		float deltaTime = GAMEINSTANCE.GetDeltaTime();
		//timeにdeltaTimeを加算していく
		time = time + deltaTime;
		if (time > 3.0f)
		{
			GAMEINSTANCE.sceneShutdown();
			return new GameOverScene;
		}
	}

	//地面より下に落ちると死亡
	if (playerPos.z <= -800)
	{
		GAMEINSTANCE.sceneShutdown();
		return new GameOverScene;
	}

	// レンダラに登録されているディレクショナルライト方向を取得
	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	lightDir.Normalize();

	// シャドウマップレンダリングのためのライト情報渡す
	RENDERER->SetDepthSetting(playerPos, lightDir, Vector3::UnitZ, 5000.0f);

	return this;
}

void TestScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//ゲームシステム関連描画
	GAMEINSTANCE.GetRenderer()->Draw();

	static float anim = 0.0f;
	anim += 0.01f;

	// 2D描画
	RENDERER->SpriteDrawBegin();
	//RENDERER->DrawTexture(mTexture, 3, 2, 2, Vector2(100.0, 100.0), 1.0f, 1.0f);
	//ヤドンの画像
	/*RENDERER->DrawTexture(mTexture, Vector2(100.0, 100.0));*/
	char buf[256];

	//画面の時間の表示
	/*sprintf(buf, "anim(%f)", anim);*/
	/*mFont->TextDraw(200, 300, buf);*/

	RENDERER->SpriteDrawEnd();

	//画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}