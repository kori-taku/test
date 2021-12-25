#include "System.h"
#include "PolRead_DX/PolRead.h"
#include "EndingSys.h"



// コンストラクタ
EndingSys::EndingSys()
{
    endingFlag = EndingFlag::INIT;
    endingKeyIn = true;
}


// タイトル関数
SceneFlag EndingSys::Ending(void)
{
	switch (endingFlag)
	{
	case EndingFlag::INIT:
		// エンディング初期化
		EndingInit();
		break;
	case EndingFlag::MAIN:
		// エンディングメイン
		EndingMain();
		break;
	case EndingFlag::END:
		// エンディング終了処理
		EndingEnd();
		break;
	default:
		break;
	}

	return sceneFlag;
}

void EndingSys::EndingInit(void)
{
	// メインに切り替え
 	endingFlag = EndingFlag::MAIN;
	sceneFlag = SceneFlag::ENDING;

	if (dataFlag == false)
	{
		// テクスチャデータの読み込み
		textureNo = directXBase.SetTextureFile("data/Ending.png");

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
	endingKeyFlag = true;
}

void EndingSys::EndingMain(void)
{
	// キー入力
	endingKeyIn = GetAsyncKeyState(VK_HOME);
	if (endingKeyIn & 0x8000)
	{
		if (endingKeyFlag == false)
		{
			endingKeyFlag = true;
			endingFlag = EndingFlag::END;
		}
	}
	else
	{
		// 押してないときの処理
		endingKeyFlag = false;
	}

	// 判定

	// 描画処理
	SysDraw3D();
}

void EndingSys::EndingEnd(void)
{
	// フラグ切り替え
	endingFlag = EndingFlag::INIT;

	// ゲームフラグ切り替え
	sceneFlag = SceneFlag::TITLE;

	// 登録モデルの非表示
	directXBase.SetModelObjOff();

}

void EndingSys::SysDraw3D(void)
{
	//描画処理
	directXBase.Render();
}



