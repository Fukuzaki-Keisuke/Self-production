#pragma once
#include "Game.h"
#include "SceneBase.h"

class GameClearScene : public SceneBase
{
public:
	GameClearScene();
	~GameClearScene();

	SceneBase* update();
	void	   draw();

private:
	class Texture* mTexture;
	class Texture* mTexture2;
	class Texture* mTexture3;
};