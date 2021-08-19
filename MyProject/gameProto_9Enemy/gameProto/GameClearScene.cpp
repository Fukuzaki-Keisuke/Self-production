#include "Renderer.h"
#include "GameClearScene.h"
#include "Texture.h"
#include "Input.h"
#include "TestScene.h"
#include "TitleScene.h"

GameClearScene::GameClearScene()
{
	mTexture = RENDERER->GetTexture("assets/GameClear/GameClear.png");
	mTexture2 = RENDERER->GetTexture("assets/Title/PtS.png");
	/*mTexture3 = RENDERER->GetTexture("assets/Title/TitleChan.png");*/
}

GameClearScene::~GameClearScene()
{
}

SceneBase* GameClearScene::update()
{
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		return new TitleScene;
	}
	return this;
}

void GameClearScene::draw()
{
	static float anim = 0.0f;
	anim += 0.01;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();
	//�Q�[���V�X�e���֘A�`��
	GAMEINSTANCE.GetRenderer()->Draw();

	// 2D�`��
	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(mTexture, Vector2(640.0, 385.0));
	RENDERER->DrawTexture(mTexture2, Vector2(640.0, 450.0), 0.2f * fabs(sinf(anim)) + 1.0f, fabs(sinf(anim)));
	/*RENDERER->DrawTexture(mTexture3, Vector2(640.0, 355.0), 1.0f, fabs(sinf(anim)));*/
	RENDERER->SpriteDrawEnd();

	//��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
