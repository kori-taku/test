#include "System.h"
#include "PolRead_DX/PolRead.h"
#include "TitleSys.h"


// �R���X�g���N�^
TitleSys::TitleSys()
{

	titleFlag = TitleFlag::INIT;
	titleKeyFlag = true;

}


// �^�C�g���V�X�e��
SceneFlag TitleSys::Title(void)
{
	switch ( titleFlag )
	{
	case TitleFlag::INIT:
		// �^�C�g��������
		TitleInit();
		break;
	case TitleFlag::MAIN:
		// �^�C�g�����C��
		TitleMain();
		break;
	case TitleFlag::END:
		TitleEnd();
		// �^�C�g���I������
		break;
	default:
		break;
	}

	// �����I��
	//sceneFlag = SceneFlag::EXIT;

	return sceneFlag;
}


void TitleSys::TitleInit(void)
{
	// �^�C�g�����C���ɐ؂�ւ�
	titleFlag = TitleFlag::MAIN;
	sceneFlag = SceneFlag::TITLE;

	// �摜�f�[�^�o�^
	if (dataFlag == false)
	{
		// �e�N�X�`���f�[�^�̓ǂݍ���
		textureNo = directXBase.SetTextureFile("data/Title.png");
		// �e�N�X�`���f�[�^�̓ǂݍ���
		// textureNo = directXBase.SetTextureFile("data/block.png");

		// ���f���f�[�^�̓ǂݍ���
		ModelData* modelObj = ModelDataLoad("data/Sprite.pol");

		// ���f���f�[�^�̓o�^
		modelNo = directXBase.SetModel(modelObj);

		// �V�F�[�_�[���R���p�C���i���X�g�o�^�j
		shaderNo = directXBase.SetShaderFile(L"hlsl/simpleTexVS.hlsl", L"hlsl/simpleTexPS.hlsl");

		// ���f���I�u�W�F�N�g�쐬
		modelObjNo = directXBase.AddModelObj();
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
	titleKeyFlag = true;

}


void TitleSys::TitleMain(void)
{
	// �L�[����
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
		// �����ĂȂ��Ƃ��̏���
		titleKeyFlag = false;
	}

	// ����

	// �`�揈��
	SysDraw3D();

}



void TitleSys::TitleEnd(void)
{
	// �^�C�g���t���O�؂�ւ�
	titleFlag = TitleFlag::INIT;

	// �Q�[���t���O�؂�ւ�
	sceneFlag = SceneFlag::GAME;

	// �o�^���f���̔�\��
	directXBase.SetModelObjOff();

}



//----------------------------------------------------------------------
// �`�揈��
//----------------------------------------------------------------------
void TitleSys::SysDraw3D(void)
{
	//�`�揈��
	directXBase.Render();
}
