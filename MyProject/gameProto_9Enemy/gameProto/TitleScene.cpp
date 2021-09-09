#include "Renderer.h"
#include "TitleScene.h"
#include "Texture.h"
#include "Input.h"
#include "TestScene.h"

TitleScene::TitleScene()
{
	//mTexture = RENDERER->GetTexture("assets/Title/Title.png");
	mTexture2 = RENDERER->GetTexture("assets/Title/PtS.png");
	mTexture3 = RENDERER->GetTexture("assets/Title/Title1.png");
}

TitleScene::~TitleScene()
{
}

SceneBase* TitleScene::update()
{
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		return new TestScene;
	}
	return this;
}

void TitleScene::draw()
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
	//RENDERER->DrawTexture(mTexture, Vector2(640.0, 385.0));
	RENDERER->DrawTexture(mTexture2, Vector2(640.0, 450.0), 0.2f * fabs(sinf(anim)) + 1.0f, fabs(sinf(anim)));
	RENDERER->DrawTexture(mTexture3, Vector2(640.0, 355.0), 1.0f, fabs(sinf(anim)));
	RENDERER->SpriteDrawEnd();

	//画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
