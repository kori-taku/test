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
	// �V�[���t���O
	SceneFlag sceneFlag = SceneFlag::TITLE;

	// �^�C�g���Ǘ��t���O
	TitleFlag titleFlag = TitleFlag::INIT;

	bool dataFlag = false;
	
	int textureNo;
	int modelNo;
	int shaderNo;
	int modelObjNo;

	// �L�[����
	short titleKeyIn;
	int   titleKeyFlag; //�������ςȂ��t���O

public:
	// �R���X�g���N�^
	TitleSys();

	// �^�C�g���֐�
	SceneFlag Title(void);
	void TitleInit(void);
	void TitleMain(void);
	void TitleEnd(void);
	void SysDraw3D(void);

};