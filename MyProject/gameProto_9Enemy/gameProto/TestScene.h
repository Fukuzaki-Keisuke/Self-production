#pragma once
#include "Game.h"
#include "SceneBase.h"

class TestScene : public SceneBase
{
public:
	TestScene();
	~TestScene();

	SceneBase* update();
	void       draw();

	class DebugGrid* mGrid;
	class LevelManager* mLevel;

	class Texture* mTexture;
	class Texture* mTexture1;
	class Texture* mTexture2;
	class Texture* mTexture3;

	class BitmapText* mFont;

	//プレイヤー
	class PlayerActor* mPlayer;

	//sampleエネミー
	class BlackKnightActor* mEnemy;
	//近接のみのエネミー
	class NearEnemy* mNearEnemy;


	class EnemyActorBase* mEnemyBase;
};