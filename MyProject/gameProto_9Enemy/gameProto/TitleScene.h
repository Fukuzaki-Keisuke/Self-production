#pragma once
#include "Game.h"
#include "SceneBase.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();

	SceneBase* update();
	void	   draw();

private:
	class Texture* mTexture;
	class Texture* mTexture2;
	class Texture* mTexture3;
};