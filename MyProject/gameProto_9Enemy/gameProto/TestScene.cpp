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
	//�v���[���[�̏����ʒu
	mPlayer = new PlayerActor;
	mPlayer->SetPosition(Vector3(1000, -1800, 40));

	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.5f, 0.5f, 0.5f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection    = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor    = Vector3(0.8f, 0.8f, 0.8f);

	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid( 1000.0f, 30, color );

	//�J�����n
	ThirdPersonCamera* c = new ThirdPersonCamera(mPlayer);
	c->Init(Vector3(300, 0, 300), Vector3(0, 0, 10), Vector3(0, 0, 1));
	//�J�����Ƃ̋���
	c->SetCameraLength(600.0f);

	AUDIO->GetSound("assets/audio/jump.wav");
	AUDIO->GetSound("assets/audio/coin.wav");
	AUDIO->GetMusic("assets/audio/loopMusic.wav");
	//AUDIO->PlayMusic("assets/audio/loopMusic.wav");

	//�}�b�v��gpmesh�ǂݍ���
	//���x���ǂݍ���
	/*mLevel = new LevelManager;*/
	LevelActor* level = new LevelActor;
	Vector3 offs(-60, 380, 20);
	level->LoadLevel("assets/Map/MapStaticMesh.gpmesh", "assets/Map/collision.json", offs);

	//���h���̃e�N�X�`��(�g���ĂȂ�)
	mTexture = RENDERER->GetTexture("assets/texture.png");
 	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "assets/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	//�����G�𕡐��p�ӂ���
	//�G�l�~�[�̏����ʒu

	//////�G���G�l�~�[1 ����
	//mEnemyBase = new BlackKnightActor;
	//mEnemyBase->SetPosition(Vector3(-800, 1300, 0));

	//�G���G�l�~�[2 �E��
	mEnemyBase = new BlackKnightActor;
	mEnemyBase->SetPosition(Vector3(1000, 1300, 0));

	//�ߐڂ݂̂̃G�l�~�[
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

	//�v���C���[�̈ʒu�����
	Vector3 playerPos;
	playerPos = GAMEINSTANCE.GetPlayerPos();

	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
 		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}

	//�Q�[���N���A�V�[���Ɉڍs

	//�G��S���|������N���A
	if (EnemyActorBase::GetEnemyCount() <= 0)
	{
		//���Ԃ̌v��(static�͑O�̃t���[���̎��Ԃ�ێ�����)
		static float time = 0.0f;
		//deltaTime�̎擾
		float deltaTime = GAMEINSTANCE.GetDeltaTime();
		//time��deltaTime�����Z���Ă���
		time = time + deltaTime;
		if (time > 3.0f)
		{
			GAMEINSTANCE.sceneShutdown();
			return new GameClearScene;
		}
	}

	//�Q�[���I�[�o�[�V�[���Ɉڍs

	//�v���C���[�̗̑͂�0�ɂȂ�Ǝ��S
	if (mPlayer->GetmPHitPoint() <= 0)
	{
		//���Ԃ̌v��
		static float time = 0.0f;
		//deltaTime�̎擾(static�͑O�̃t���[���̎��Ԃ�ێ�����)
		float deltaTime = GAMEINSTANCE.GetDeltaTime();
		//time��deltaTime�����Z���Ă���
		time = time + deltaTime;
		if (time > 3.0f)
		{
			GAMEINSTANCE.sceneShutdown();
			return new GameOverScene;
		}
	}

	//�n�ʂ�艺�ɗ�����Ǝ��S
	if (playerPos.z <= -800)
	{
		GAMEINSTANCE.sceneShutdown();
		return new GameOverScene;
	}

	// �����_���ɓo�^����Ă���f�B���N�V���i�����C�g�������擾
	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	lightDir.Normalize();

	// �V���h�E�}�b�v�����_�����O�̂��߂̃��C�g���n��
	RENDERER->SetDepthSetting(playerPos, lightDir, Vector3::UnitZ, 5000.0f);

	return this;
}

void TestScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();

	//�Q�[���V�X�e���֘A�`��
	GAMEINSTANCE.GetRenderer()->Draw();

	static float anim = 0.0f;
	anim += 0.01f;

	// 2D�`��
	RENDERER->SpriteDrawBegin();
	//RENDERER->DrawTexture(mTexture, 3, 2, 2, Vector2(100.0, 100.0), 1.0f, 1.0f);
	//���h���̉摜
	/*RENDERER->DrawTexture(mTexture, Vector2(100.0, 100.0));*/
	char buf[256];

	//��ʂ̎��Ԃ̕\��
	/*sprintf(buf, "anim(%f)", anim);*/
	/*mFont->TextDraw(200, 300, buf);*/

	RENDERER->SpriteDrawEnd();

	//��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}