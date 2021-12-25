#pragma once

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <wrl.h>
#include <vector>


#pragma comment(lib, "winmm.lib")

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;


enum class SceneFlag
{
	TITLE,
	GAME,
	ENDING,
	EXIT,
	MAX
};



#define FPSCOUNT (1000/60)


class TitleSys;
class GameMainSys;
class EndingSys;


// メインシステムクラス
// FPS管理
// タイトル・ゲームメイン・エンディング管理
class MainSys
{
private:
	// システム起動フラグ
	bool systemFlag = true;

	// ゲームシーン
	SceneFlag sceneFlag = SceneFlag::TITLE;

	// FPS 管理
	DWORD fpsChk1; // スタート時間
	DWORD fpsChk2; // 経過時間

	// タイトル
	TitleSys	*titleSys;
	// ゲームメイン
	GameMainSys *gameMainSys;
	// エンディング
	EndingSys	*endingSys;

public:

	// コンストラクタ
	MainSys();


	// ゲームシステム
	bool GameSystem();

};