#pragma once
#include "Game.h"
#include "SceneBase.h"

class GameOverScene : public SceneBase
{
public:
	GameOverScene();
	~GameOverScene();

	SceneBase* update();
	void	   draw();

private:
	class Texture* mTexture;
	class Texture* mTexture2;
	class Texture* mTexture3;
};