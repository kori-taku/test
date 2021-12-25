#pragma once

#include "MainSys.h"

// 前方宣言


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
	// シーンフラグ
	SceneFlag sceneFlag = SceneFlag::GAME;
	// ゲームメインフラグ
	GameMainFlag gameMainFlag = GameMainFlag::INIT;



	bool dataFlag = false;

	int textureNo;
	int modelNo;
	int shaderNo;

	int gameKeyIn;
	int gameKeyFlag;

public:
	GameMainSys();

	// タイトル関数
	SceneFlag Game(void);
	void GameInit(void);
	void GameMain(void);
	void GameEnd(void);
	void SysDraw3D(void);

};