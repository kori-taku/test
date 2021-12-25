#pragma once
#include "D3D12AppBase.h"
#include <DirectXMath.h>

class ModelObj
{
private:
public:
    int dispFlag = true;

    int constantNo;
    int textureNo;
    int shaderNo;
    int bufferNo;

    // ���[���h�}�g���b�N�X
    DirectX::XMFLOAT4X4 mtxWorld;
};