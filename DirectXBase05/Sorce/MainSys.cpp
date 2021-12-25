#include "MainSys.h"

#include "TitleSys.h"
#include "GameMainSys.h"
#include "EndingSys.h"

// �R���X�g���N�^
MainSys :: MainSys()
{
	titleSys = new TitleSys();
	gameMainSys = new GameMainSys();
	endingSys = new EndingSys();
}


// �Q�[���V�X�e��
bool MainSys :: GameSystem()
{

	// �o�ߎ��Ԃ̎擾
	fpsChk2 = timeGetTime();

	// �X�^�[�g����̌o�ߎ��Ԍv�Z
	DWORD chkTime = fpsChk2 - fpsChk1;

	// FPS���Ԍo�߃`�F�b�N
	if (chkTime < FPSCOUNT)
	{
		// 1/60 �o�߂��Ă��Ȃ��̂�
		// �����𔲂���
		return systemFlag;
	}

	// �X�^�[�g�^�C���̍X�V
	fpsChk1 = fpsChk2;


	// �Q�[����������
	switch (sceneFlag)
	{
	case SceneFlag::TITLE:
		sceneFlag = titleSys->Title();
		break;

	case SceneFlag::GAME:
		sceneFlag = gameMainSys->Game();
		break;

	case SceneFlag::ENDING:
		sceneFlag = endingSys->Ending();
		break;

	case SceneFlag::EXIT:
		systemFlag = false;
		break;
	}


	return systemFlag;
};
