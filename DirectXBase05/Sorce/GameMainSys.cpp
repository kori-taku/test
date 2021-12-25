#include "System.h"
#include "PolRead_DX/PolRead.h"
#include "GameMainSys.h"


// �R���X�g���N�^
GameMainSys::GameMainSys()
{
	gameMainFlag = GameMainFlag::INIT;
	gameKeyFlag = true;


}



// �V�X�e��
SceneFlag GameMainSys::Game(void)
{
	switch (gameMainFlag)
	{
	case GameMainFlag::INIT:
		// �Q�[�����C��������
		GameInit();
		break;
	case GameMainFlag::MAIN:
		// �Q�[�����C��
		GameMain();
		break;
	case GameMainFlag::END:
		GameEnd();
		// �Q�[�����C���I������
		break;
	default:
		break;
	}

	return sceneFlag;
}


void GameMainSys::GameInit(void)
{
	// �Q�[�����C���ɐ؂�ւ�
	gameMainFlag = GameMainFlag::MAIN;
	sceneFlag = SceneFlag::GAME;

	if (dataFlag == false)
	{
		// �e�N�X�`���f�[�^�̓ǂݍ���
		textureNo = directXBase.SetTextureFile("data/block.png");

		// ���f���f�[�^�̓ǂݍ���
		ModelData* modelObj = ModelDataLoad("data/Sprite.pol");

		// ���f���f�[�^�̓o�^
		modelNo = directXBase.SetModel(modelObj);

		// �V�F�[�_�[���R���p�C���i���X�g�o�^�j
		shaderNo = directXBase.SetShaderFile(L"hlsl/simpleTexVS.hlsl", L"hlsl/simpleTexPS.hlsl");

		// ���f���I�u�W�F�N�g�쐬
		int modelObjNo = directXBase.AddModelObj();
		directXBase.InitModelObj(
			modelObjNo,
			textureNo,
			shaderNo,
			modelNo);


		// �e��f�[�^�ݒ�
		directXBase.PrepareInit();


		dataFlag = true;
	}
	

	directXBase.SetModelObjDisp(modelNo, true);



	// �ŏ��̓L�[�������Ȃ��悤�ɂ���B
	gameKeyFlag = true;
}


void GameMainSys::GameMain(void)
{
	// �L�[����
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
		// �����ĂȂ��Ƃ��̏���
		gameKeyFlag = false;
	}

	// �`�揈��
	SysDraw3D();

}



void GameMainSys::GameEnd(void)
{
	// �Q�[�����C���t���O�؂�ւ�
	gameMainFlag = GameMainFlag::INIT;

	// �Q�[���t���O�؂�ւ�
	sceneFlag = SceneFlag::ENDING;

	// �o�^���f���̔�\��
	directXBase.SetModelObjOff();

}


//----------------------------------------------------------------------
// �`�揈��
//----------------------------------------------------------------------
void GameMainSys::SysDraw3D(void)
{
	//�`�揈��
	directXBase.Render();
}
