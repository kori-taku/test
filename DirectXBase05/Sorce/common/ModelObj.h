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

    // ワールドマトリックス
    DirectX::XMFLOAT4X4 mtxWorld;
};