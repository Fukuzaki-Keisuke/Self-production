#include "Renderer.h"
#include "GameOverScene.h"
#include "Texture.h"
#include "Input.h"
#include "TestScene.h"
#include "TitleScene.h"

GameOverScene::GameOverScene()
{
	mTexture = RENDERER->GetTexture("assets/GameOver/GameOver.png");
	mTexture2 = RENDERER->GetTexture("assets/Title/PtS.png");
	/*mTexture3 = RENDERER->GetTexture("assets/Title/TitleChan.png");*/
}

GameOverScene::~GameOverScene()
{
}

SceneBase* GameOverScene::update()
{
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		return new TitleScene;
	}
	return this;
}

void GameOverScene::draw()
{
	static float anim = 0.0f;
	anim += 0.01;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();
	//ゲームシステム関連描画
	GAMEINSTANCE.GetRenderer()->Draw();

	// 2D描画
	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(mTexture, Vector2(640.0, 385.0));
	RENDERER->DrawTexture(mTexture2, Vector2(640.0, 450.0), 0.2f * fabs(sinf(anim)) + 1.0f, fabs(sinf(anim)));
	//RENDERER->DrawTexture(mTexture3, Vector2(640.0, 355.0), 1.0f, fabs(sinf(anim)));
	RENDERER->SpriteDrawEnd();

	//画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
