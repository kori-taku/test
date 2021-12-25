#pragma once

#include "MainSys.h"

// �O���錾


enum class GameMainFlag
{
	INIT,
	MAIN,
	END,
	MAX,
};


class GameMainSys
{
private:
	// �V�[���t���O
	SceneFlag sceneFlag = SceneFlag::GAME;
	// �Q�[�����C���t���O
	GameMainFlag gameMainFlag = GameMainFlag::INIT;



	bool dataFlag = false;

	int textureNo;
	int modelNo;
	int shaderNo;

	int gameKeyIn;
	int gameKeyFlag;

public:
	GameMainSys();

	// �^�C�g���֐�
	SceneFlag Game(void);
	void GameInit(void);
	void GameMain(void);
	void GameEnd(void);
	void SysDraw3D(void);

};