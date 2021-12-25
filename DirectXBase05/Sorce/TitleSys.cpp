#include "System.h"
#include "PolRead_DX/PolRead.h"
#include "TitleSys.h"


// コンストラクタ
TitleSys::TitleSys()
{

	titleFlag = TitleFlag::INIT;
	titleKeyFlag = true;

}


// タイトルシステム
SceneFlag TitleSys::Title(void)
{
	switch ( titleFlag )
	{
	case TitleFlag::INIT:
		// タイトル初期化
		TitleInit();
		break;
	case TitleFlag::MAIN:
		// タイトルメイン
		TitleMain();
		break;
	case TitleFlag::END:
		TitleEnd();
		// タイトル終了処理
		break;
	default:
		break;
	}

	// 強制終了
	//sceneFlag = SceneFlag::EXIT;

	return sceneFlag;
}


void TitleSys::TitleInit(void)
{
	// タイトルメインに切り替え
	titleFlag = TitleFlag::MAIN;
	sceneFlag = SceneFlag::TITLE;

	// 画像データ登録
	if (dataFlag == false)
	{
		// テクスチャデータの読み込み
		textureNo = directXBase.SetTextureFile("data/Title.png");
		// テクスチャデータの読み込み
		// textureNo = directXBase.SetTextureFile("data/block.png");

		// モデルデータの読み込み
		ModelData* modelObj = ModelDataLoad("data/Sprite.pol");

		// モデルデータの登録
		modelNo = directXBase.SetModel(modelObj);

		// シェーダーをコンパイル（リスト登録）
		shaderNo = directXBase.SetShaderFile(L"hlsl/simpleTexVS.hlsl", L"hlsl/simpleTexPS.hlsl");

		// モデルオブジェクト作成
		modelObjNo = directXBase.AddModelObj();
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
	titleKeyFlag = true;

}


void TitleSys::TitleMain(void)
{
	// キー入力
	titleKeyIn = GetAsyncKeyState(VK_HOME);
	if (titleKeyIn & 0x8000)
	{
		if(titleKeyFlag == false)
		{
			titleKeyFlag = true;
			titleFlag = TitleFlag::END;
		}
	}
	else
	{
		// 押してないときの処理
		titleKeyFlag = false;
	}

	// 判定

	// 描画処理
	SysDraw3D();

}



void TitleSys::TitleEnd(void)
{
	// タイトルフラグ切り替え
	titleFlag = TitleFlag::INIT;

	// ゲームフラグ切り替え
	sceneFlag = SceneFlag::GAME;

	// 登録モデルの非表示
	directXBase.SetModelObjOff();

}



//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void TitleSys::SysDraw3D(void)
{
	//描画処理
	directXBase.Render();
}
