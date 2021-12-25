#include "MainSys.h"

#include "TitleSys.h"
#include "GameMainSys.h"
#include "EndingSys.h"

// コンストラクタ
MainSys :: MainSys()
{
	titleSys = new TitleSys();
	gameMainSys = new GameMainSys();
	endingSys = new EndingSys();
}


// ゲームシステム
bool MainSys :: GameSystem()
{

	// 経過時間の取得
	fpsChk2 = timeGetTime();

	// スタートからの経過時間計算
	DWORD chkTime = fpsChk2 - fpsChk1;

	// FPS時間経過チェック
	if (chkTime < FPSCOUNT)
	{
		// 1/60 経過していないので
		// 処理を抜ける
		return systemFlag;
	}

	// スタートタイムの更新
	fpsChk1 = fpsChk2;


	// ゲーム処理分岐
	switch (sceneFlag)
	{
	case SceneFlag::TITLE:
		sceneFlag = titleSys->Title();
		break;

	case SceneFlag::GAME:
		sceneFlag = gameMainSys->Game();
		break;

	case SceneFlag::ENDING:
		sceneFlag = endingSys->Ending();
		break;

	case SceneFlag::EXIT:
		systemFlag = false;
		break;
	}


	return systemFlag;
};
