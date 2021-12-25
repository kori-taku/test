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
    // �V�[���t���O
    SceneFlag sceneFlag = SceneFlag::ENDING;

    // �^�C�g���Ǘ��t���O
    EndingFlag endingFlag = EndingFlag::INIT;


    bool dataFlag = false;


    // �L�[����
    short endingKeyIn;
    int   endingKeyFlag; //�������ςȂ��t���O


    int textureNo;
    int modelNo;
    int shaderNo;
    int modelObjNo;


public:
    // �R���X�g���N�^
    EndingSys();

    // �^�C�g���֐�
    SceneFlag Ending(void);
    void EndingInit(void);
    void EndingMain(void);
    void EndingEnd(void);
    void SysDraw3D(void);

};