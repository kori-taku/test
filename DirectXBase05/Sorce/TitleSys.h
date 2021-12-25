#pragma once

#include "MainSys.h"

enum class TitleFlag
{
	INIT,
	MAIN,
	END,
	MAX,
};

class TitleSys
{
public:
private:
	// シーンフラグ
	SceneFlag sceneFlag = SceneFlag::TITLE;

	// タイトル管理フラグ
	TitleFlag titleFlag = TitleFlag::INIT;

	bool dataFlag = false;
	
	int textureNo;
	int modelNo;
	int shaderNo;
	int modelObjNo;

	// キー入力
	short titleKeyIn;
	int   titleKeyFlag; //押しっぱなしフラグ

public:
	// コンストラクタ
	TitleSys();

	// タイトル関数
	SceneFlag Title(void);
	void TitleInit(void);
	void TitleMain(void);
	void TitleEnd(void);
	void SysDraw3D(void);

};