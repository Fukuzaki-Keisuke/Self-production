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
	class BitmapText* mFont;

	//�v���C���[
	class PlayerActor* mPlayer;

	//sample�G�l�~�[
	class BlackKnightActor* mEnemy;
	//�ߐڂ݂̂̃G�l�~�[
	class NearEnemy* mNearEnemy;


	class EnemyActorBase* mEnemyBase;
};