#include "System.h"
#include "PolRead_DX/PolRead.h"
#include "EndingSys.h"



// �R���X�g���N�^
EndingSys::EndingSys()
{
    endingFlag = EndingFlag::INIT;
    endingKeyIn = true;
}


// �^�C�g���֐�
SceneFlag EndingSys::Ending(void)
{
	switch (endingFlag)
	{
	case EndingFlag::INIT:
		// �G���f�B���O������
		EndingInit();
		break;
	case EndingFlag::MAIN:
		// �G���f�B���O���C��
		EndingMain();
		break;
	case EndingFlag::END:
		// �G���f�B���O�I������
		EndingEnd();
		break;
	default:
		break;
	}

	return sceneFlag;
}

void EndingSys::EndingInit(void)
{
	// ���C���ɐ؂�ւ�
 	endingFlag = EndingFlag::MAIN;
	sceneFlag = SceneFlag::ENDING;

	if (dataFlag == false)
	{
		// �e�N�X�`���f�[�^�̓ǂݍ���
		textureNo = directXBase.SetTextureFile("data/Ending.png");

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
	endingKeyFlag = true;
}

void EndingSys::EndingMain(void)
{
	// �L�[����
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
		// �����ĂȂ��Ƃ��̏���
		endingKeyFlag = false;
	}

	// ����

	// �`�揈��
	SysDraw3D();
}

void EndingSys::EndingEnd(void)
{
	// �t���O�؂�ւ�
	endingFlag = EndingFlag::INIT;

	// �Q�[���t���O�؂�ւ�
	sceneFlag = SceneFlag::TITLE;

	// �o�^���f���̔�\��
	directXBase.SetModelObjOff();

}

void EndingSys::SysDraw3D(void)
{
	//�`�揈��
	directXBase.Render();
}



