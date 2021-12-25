#pragma once

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <wrl.h>
#include <vector>


#pragma comment(lib, "winmm.lib")

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;


enum class SceneFlag
{
	TITLE,
	GAME,
	ENDING,
	EXIT,
	MAX
};



#define FPSCOUNT (1000/60)


class TitleSys;
class GameMainSys;
class EndingSys;


// ���C���V�X�e���N���X
// FPS�Ǘ�
// �^�C�g���E�Q�[�����C���E�G���f�B���O�Ǘ�
class MainSys
{
private:
	// �V�X�e���N���t���O
	bool systemFlag = true;

	// �Q�[���V�[��
	SceneFlag sceneFlag = SceneFlag::TITLE;

	// FPS �Ǘ�
	DWORD fpsChk1; // �X�^�[�g����
	DWORD fpsChk2; // �o�ߎ���

	// �^�C�g��
	TitleSys	*titleSys;
	// �Q�[�����C��
	GameMainSys *gameMainSys;
	// �G���f�B���O
	EndingSys	*endingSys;

public:

	// �R���X�g���N�^
	MainSys();


	// �Q�[���V�X�e��
	bool GameSystem();

};