#pragma once

#include "MainSys.h"


enum class EndingFlag
{
    INIT,
    MAIN,
    END,
    MAX,
};

class EndingSys
{
private:
    // シーンフラグ
    SceneFlag sceneFlag = SceneFlag::ENDING;

    // タイトル管理フラグ
    EndingFlag endingFlag = EndingFlag::INIT;


    bool dataFlag = false;


    // キー入力
    short endingKeyIn;
    int   endingKeyFlag; //押しっぱなしフラグ


    int textureNo;
    int modelNo;
    int shaderNo;
    int modelObjNo;


public:
    // コンストラクタ
    EndingSys();

    // タイトル関数
    SceneFlag Ending(void);
    void EndingInit(void);
    void EndingMain(void);
    void EndingEnd(void);
    void SysDraw3D(void);

};