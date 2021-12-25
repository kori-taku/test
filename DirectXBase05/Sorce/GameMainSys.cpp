#include "System.h"
#include "PolRead_DX/PolRead.h"
#include "GameMainSys.h"


// コンストラクタ
GameMainSys::GameMainSys()
{
	gameMainFlag = GameMainFlag::INIT;
	gameKeyFlag = true;


}



// システム
SceneFlag GameMainSys::Game(void)
{
	switch (gameMainFlag)
	{
	case GameMainFlag::INIT:
		// ゲームメイン初期化
		GameInit();
		break;
	case GameMainFlag::MAIN:
		// ゲームメイン
		GameMain();
		break;
	case GameMainFlag::END:
		GameEnd();
		// ゲームメイン終了処理
		break;
	default:
		break;
	}

	return sceneFlag;
}


void GameMainSys::GameInit(void)
{
	// ゲームメインに切り替え
	gameMainFlag = GameMainFlag::MAIN;
	sceneFlag = SceneFlag::GAME;

	if (dataFlag == false)
	{
		// テクスチャデータの読み込み
		textureNo = directXBase.SetTextureFile("data/block.png");

		// モデルデータの読み込み
		ModelData* modelObj = ModelDataLoad("data/Sprite.pol");

		// モデルデータの登録
		modelNo = directXBase.SetModel(modelObj);

		// シェーダーをコンパイル（リスト登録）
		shaderNo = directXBase.SetShaderFile(L"hlsl/simpleTexVS.hlsl", L"hlsl/simpleTexPS.hlsl");

		// モデルオブジェクト作成
		int modelObjNo = directXBase.AddModelObj();
		directXBase.InitModelObj(
			modelObjNo,
			textureNo,
			shaderNo,
			modelNo);


		// 各種データ設定
		directXBase.PrepareInit();


		dataFlag = true;
	}
	

	directXBase.SetModelObjDisp(modelNo, true);



	// 最初はキーを押せないようにする。
	gameKeyFlag = true;
}


void GameMainSys::GameMain(void)
{
	// キー入力
	gameKeyIn = GetAsyncKeyState(VK_HOME);
	if (gameKeyIn & 0x8000)
	{
		if (gameKeyFlag == false)
		{
			gameKeyFlag = true;
			gameMainFlag = GameMainFlag::END;
		}
	}
	else
	{
		// 押してないときの処理
		gameKeyFlag = false;
	}

	// 描画処理
	SysDraw3D();

}



void GameMainSys::GameEnd(void)
{
	// ゲームメインフラグ切り替え
	gameMainFlag = GameMainFlag::INIT;

	// ゲームフラグ切り替え
	sceneFlag = SceneFlag::ENDING;

	// 登録モデルの非表示
	directXBase.SetModelObjOff();

}


//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void GameMainSys::SysDraw3D(void)
{
	//描画処理
	directXBase.Render();
}
