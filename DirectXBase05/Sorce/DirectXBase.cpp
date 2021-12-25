#include <stdexcept>
#include "DirectXBase.h"
#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"

#include "PolRead_DX/PolRead.h"

void DirectXBase::Prepare()
{
    HRESULT hr;
    ComPtr<ID3DBlob> errBlob;

    CD3DX12_DESCRIPTOR_RANGE cbv, srv, sampler;
    cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // b0 レジスタ
    srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ
    sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0); // s0 レジスタ

    CD3DX12_ROOT_PARAMETER rootParams[3];
    rootParams[0].InitAsDescriptorTable(1, &cbv, D3D12_SHADER_VISIBILITY_VERTEX);
    rootParams[1].InitAsDescriptorTable(1, &srv, D3D12_SHADER_VISIBILITY_PIXEL);
    rootParams[2].InitAsDescriptorTable(1, &sampler, D3D12_SHADER_VISIBILITY_PIXEL);

    // ルートシグネチャの構築
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
    rootSigDesc.Init(
        _countof(rootParams), rootParams,   //pParameters
        0, nullptr,   //pStaticSamplers
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    );
    ComPtr<ID3DBlob> signature;
    hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
    if (FAILED(hr))
    {
        throw std::runtime_error("D3D12SerializeRootSignature faild.");
    }
    // RootSignature の生成
    hr = m_device->CreateRootSignature(
        0,
        signature->GetBufferPointer(), signature->GetBufferSize(),
        IID_PPV_ARGS(&m_rootSignature)
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("CrateRootSignature failed.");
    }

}



void DirectXBase::PrepareInit()
{
    // シェーダー設定
    SetShader();

    // 定数バッファ作成
    PrepareDescriptorHeapFor();

    // テクスチャ登録
    SetTexture();

    //ダイナミックサンプラーのディスクリプタヒープ
    D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc{
      D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
      1,
      D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
      0
    };
    m_device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_heapSampler));
    m_samplerDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);


    // サンプラーの生成
    D3D12_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D12_ENCODE_BASIC_FILTER(
        D3D12_FILTER_TYPE_LINEAR, // min
        D3D12_FILTER_TYPE_LINEAR, // mag
        D3D12_FILTER_TYPE_LINEAR, // mip
        D3D12_FILTER_REDUCTION_TYPE_STANDARD);
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.MaxLOD = FLT_MAX;
    samplerDesc.MinLOD = -FLT_MAX;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    auto descriptorSampler = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heapSampler->GetCPUDescriptorHandleForHeapStart(), SamplerDescriptorBase, m_samplerDescriptorSize);
    m_device->CreateSampler(&samplerDesc, descriptorSampler);
    m_sampler = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapSampler->GetGPUDescriptorHandleForHeapStart(), SamplerDescriptorBase, m_samplerDescriptorSize);


}


void DirectXBase::Cleanup()
{
    auto index = m_swapchain->GetCurrentBackBufferIndex();
    auto fence = m_frameFences[index];
    auto value = ++m_frameFenceValues[index];
    m_commandQueue->Signal(fence.Get(), value);
    fence->SetEventOnCompletion(value, m_fenceWaitEvent);
    WaitForSingleObject(m_fenceWaitEvent, GpuWaitTimeout);
}


void DirectXBase::MakeCommand(ComPtr<ID3D12GraphicsCommandList>& command)
{
    using namespace DirectX;


    // ルートシグネチャのセット
    command->SetGraphicsRootSignature(m_rootSignature.Get());
    // ビューポートとシザーのセット
    command->RSSetViewports(1, &m_viewport);
    command->RSSetScissorRects(1, &m_scissorRect);

    // ディスクリプタヒープをセット.
    ID3D12DescriptorHeap* heaps[] = {
      m_heapSrvCbv.Get(), m_heapSampler.Get()
    };
    command->SetDescriptorHeaps(_countof(heaps), heaps);

    // 視点の設定
    auto mtxView = XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );

    // プロジェクションマトリックスの設定
    auto mtxProj = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(45.0f),
        m_viewport.Width / m_viewport.Height,
        0.1f, 100.0f);


    // モデル毎に違うパラメータ（後で修正）
    for (int i = 0; i < m_ModelObjCount; i++)
    {

        // 描画フラグ
        int dispFlag = m_ModelObj[i].dispFlag;

        // 定数バッファの番号(フレームバッファ分 +2 )
        int constantNo = m_ModelObj[i].constantNo + 3;
        // テクスチャ
        int textureNo = m_ModelObj[i].textureNo;
        // シェーダー
        int shaderNo = m_ModelObj[i].shaderNo;
        // バーテックス・インデックスバッファ
        int bufferNo = m_ModelObj[i].bufferNo;


        // 表示チェック
        if (dispFlag == false)
        {
            continue;
        }


        // 各行列のセット.
        ShaderParameters shaderParams;
        //XMStoreFloat4x4(&shaderParams.mtxWorld,
        //    XMMatrixRotationAxis(XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),
        //        XMConvertToRadians(0)));

        XMStoreFloat4x4(&shaderParams.mtxWorld,
            XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));

        XMStoreFloat4x4(&shaderParams.mtxView, XMMatrixTranspose(mtxView));
        XMStoreFloat4x4(&shaderParams.mtxProj, XMMatrixTranspose(mtxProj));


        // 定数バッファの更新.
        auto& constantBuffer = m_constantBuffers[constantNo];
        {
            void* p;
            CD3DX12_RANGE range(0, 0);
            constantBuffer->Map(0, &range, &p);
            memcpy(p, &shaderParams, sizeof(shaderParams));
            constantBuffer->Unmap(0, nullptr);
        }


        // バッファビューの設定
        SetBufferView(bufferNo);

        // パイプラインステートのセット
        command->SetPipelineState(m_pipeline[shaderNo].Get());

        // プリミティブタイプ、頂点・インデックスバッファのセット
        command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        command->IASetVertexBuffers(0, 1, &m_vertexBufferView);
        command->IASetIndexBuffer(&m_indexBufferView);

        command->SetGraphicsRootDescriptorTable(0, m_cbViews[constantNo]);
        command->SetGraphicsRootDescriptorTable(1, m_srv[textureNo]);
        command->SetGraphicsRootDescriptorTable(2, m_sampler);
        // 描画命令
        command->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
    }

}

DirectXBase::ComPtr<ID3D12Resource1> DirectXBase::CreateBuffer(UINT bufferSize, const void* initialData)
{
    HRESULT hr;
    ComPtr<ID3D12Resource1> buffer;
    const auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    const auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
    hr = m_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer)
    );

    // 初期データの指定があるときにはコピーする
    if (SUCCEEDED(hr) && initialData != nullptr)
    {
        void* mapped;
        CD3DX12_RANGE range(0, 0);
        hr = buffer->Map(0, &range, &mapped);
        if (SUCCEEDED(hr))
        {
            memcpy(mapped, initialData, bufferSize);
            buffer->Unmap(0, nullptr);
        }
    }

    return buffer;
}


// 手動で生成版
DirectXBase::ComPtr<ID3D12Resource1> DirectXBase::CreateTexture(const std::string& fileName)
{
    ComPtr<ID3D12Resource1> texture;
    int texWidth = 0, texHeight = 0, channels = 0;
    auto* pImage = stbi_load(fileName.c_str(), &texWidth, &texHeight, &channels, 0);

    // サイズ・フォーマットからテクスチャリソースのDesc準備
    auto texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        texWidth, texHeight,
        1,  // 配列サイズ
        1   // ミップマップ数
    );

    // テクスチャ生成
    const auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    m_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &texDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&texture)
    );

    // ステージングバッファ準備
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts;
    UINT numRows;
    UINT64 rowSizeBytes, totalBytes;
    m_device->GetCopyableFootprints(&texDesc, 0, 1, 0, &layouts, &numRows, &rowSizeBytes, &totalBytes);
    ComPtr<ID3D12Resource1> stagingBuffer = CreateBuffer(UINT(totalBytes), nullptr);

    // ステージングバッファに画像をコピー
    {
        const UINT imagePitch = texWidth * sizeof(uint32_t);
        void* pBuf;
        CD3DX12_RANGE range(0, 0);
        stagingBuffer->Map(0, &range, &pBuf);
        for (UINT h = 0; h < numRows; ++h)
        {
            auto dst = static_cast<char*>(pBuf) + h * rowSizeBytes;
            auto src = pImage + h * imagePitch;
            memcpy(dst, src, imagePitch);
        }
    }

    // コマンド準備.
    ComPtr<ID3D12GraphicsCommandList> command;
    m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), nullptr, IID_PPV_ARGS(&command));
    ComPtr<ID3D12Fence1> fence;
    m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    // 転送コマンド
    D3D12_TEXTURE_COPY_LOCATION src{}, dst{};
    dst.pResource = texture.Get();
    dst.SubresourceIndex = 0;
    dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

    src.pResource = stagingBuffer.Get();
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src.PlacedFootprint = layouts;
    command->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

    // コピー後にはテクスチャとしてのステートへ.
    auto barrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
        texture.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    command->ResourceBarrier(1, &barrierTex);

    command->Close();

    // コマンドの実行
    ID3D12CommandList* cmds[] = { command.Get() };
    m_commandQueue->ExecuteCommandLists(1, cmds);
    // 完了したらシグナルを立てる.
    const UINT64 expected = 1;
    m_commandQueue->Signal(fence.Get(), expected);

    // テクスチャの処理が完了するまで待つ.
    while (expected != fence->GetCompletedValue())
    {
        Sleep(1);
    }

    stbi_image_free(pImage);
    return texture;
}


// 定数バッファ作成
void DirectXBase::PrepareDescriptorHeapFor()
{
    // CBV/SRV のディスクリプタヒープ
    //  0:シェーダーリソースビュー
    //  1,2 : 定数バッファビュー (FrameBufferCount数分使用)
    UINT count = FrameBufferCount + m_ModelObjCount + 1;
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{
      D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
      count,
      D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
      0
    };
    m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_heapSrvCbv));
    m_srvcbvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    // 定数バッファ/定数バッファビューの生成
    m_constantBuffers.resize(count);
    m_cbViews.resize(count);
    for (UINT i = 0; i < count; ++i)
    {
        UINT bufferSize = sizeof(ShaderParameters) + 255 & ~255;
        m_constantBuffers[i] = CreateBuffer(bufferSize, nullptr);

        D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
        cbDesc.BufferLocation = m_constantBuffers[i]->GetGPUVirtualAddress();
        cbDesc.SizeInBytes = bufferSize;
        CD3DX12_CPU_DESCRIPTOR_HANDLE handleCBV(m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
        m_device->CreateConstantBufferView(&cbDesc, handleCBV);

        m_cbViews[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapSrvCbv->GetGPUDescriptorHandleForHeapStart(), ConstantBufferDescriptorBase + i, m_srvcbvDescriptorSize);
    }

}


// テクスチャファイルセット
int DirectXBase::SetTextureFile(const std::string& texName)
{
    // リストの更新
    m_textureCount++;
    m_textureList.resize(m_textureCount);

    int count = m_textureCount - 1;

    // データ読み込み
    // テクスチャの生成
    m_textureList[count] = CreateTexture(texName);

    return count;
}


// テクスチャセット
bool DirectXBase::SetTexture()
{
    // エラーチェック
    if (m_textureCount < 1 )
    {
        OutputDebugStringW(L"DirectXBase::SetTexture  m_textureCount エラー");
        return false;
    }


    m_srv.resize(m_textureCount);
    for (int i = 0; i < m_textureCount; i++)
    {
        // テクスチャからシェーダーリソースビューの準備.
        auto srvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heapSrvCbv->GetCPUDescriptorHandleForHeapStart(), TextureSrvDescriptorBase + i, m_srvcbvDescriptorSize);
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        m_device->CreateShaderResourceView(m_textureList[i].Get(), &srvDesc, srvHandle);
        m_srv[i] = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heapSrvCbv->GetGPUDescriptorHandleForHeapStart(), TextureSrvDescriptorBase + i, m_srvcbvDescriptorSize);
    }

    return true;
}






// バーテックスバッファ・インデックスバッファセット
int DirectXBase::SetModel(ModelData* model)
{

    // リスト処理
    m_bufferCount++;
    m_vbufferList.resize(m_bufferCount);
    m_vSizeList.resize(m_bufferCount);
    m_ibufferList.resize(m_bufferCount);
    m_iSizeList.resize(m_bufferCount);

    int count = m_bufferCount - 1;


    // 頂点データの数
    m_vSizeList[count] = sizeof(OBJ_VERTEX_BUF)
        * model->vertexObj->dat_cnt;

    m_iSizeList[count] = sizeof(unsigned int)
        * model->vertexObj->dat_cnt;

    // 頂点バッファとインデックスバッファの生成.
    m_vbufferList[count] = CreateBuffer(
        m_vSizeList[count],
        model->vertexObj->obj_dat);

    m_ibufferList[count] = CreateBuffer(
        m_iSizeList[count],
        model->vertexObj->obj_index);

    return count;
}


// バーテックスバッファ・インデックスバッファビューセット
bool DirectXBase::SetBufferView(int bufferNo)
{
    // エラーチェック
    if (bufferNo < 0 || bufferNo >= m_bufferCount)
    {
        OutputDebugStringW(L"DirectXBase::SetBufferView  bufferNo エラー");
        return false;
    }

    // 各バッファのビューを生成.
    m_vertexBufferView.BufferLocation = m_vbufferList[bufferNo]->GetGPUVirtualAddress();
    m_vertexBufferView.SizeInBytes    = m_vSizeList[bufferNo];
    m_vertexBufferView.StrideInBytes  = sizeof(OBJ_VERTEX_BUF);
    m_indexBufferView.BufferLocation  = m_ibufferList[bufferNo]->GetGPUVirtualAddress();
    m_indexBufferView.SizeInBytes     = m_iSizeList[bufferNo];
    m_indexBufferView.Format          = DXGI_FORMAT_R32_UINT;

    // データ数
    m_indexCount = m_iSizeList[bufferNo] / sizeof(unsigned int);

    return true;
}




// シェーダーファイル設定
int DirectXBase::SetShaderFile(const std::wstring& vsName, const std::wstring& psName)
{
    // バッファの更新
    m_shaderCount++;
    m_vsList.resize(m_shaderCount);
    m_psList.resize(m_shaderCount);
    m_pipeline.resize(m_shaderCount);

    // カウンター
    int count = m_shaderCount - 1;

    // シェーダーをコンパイル.
    HRESULT hr;
    ComPtr<ID3DBlob> errBlob;
    hr = CompileShaderFromFile(vsName, L"vs_6_0", m_vsList[count], errBlob);
    if (FAILED(hr))
    {
        OutputDebugStringA((const char*)errBlob->GetBufferPointer());
        count = -1;
    }
    hr = CompileShaderFromFile(psName, L"ps_6_0", m_psList[count], errBlob);
    if (FAILED(hr))
    {
        OutputDebugStringA((const char*)errBlob->GetBufferPointer());
        count = -1;
    }

    return count;
}


// シェーダーセット
bool DirectXBase::SetShader()
{

    if ( m_shaderCount < 1)
    {
        OutputDebugStringW( L"DirectXBase::SetShader shaderNo エラー" );
        return false;
    }

    // インプットレイアウト
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0, offsetof(OBJ_VERTEX_BUF, obj_v), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
      { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,   0, offsetof(OBJ_VERTEX_BUF, obj_n), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,      0, offsetof(OBJ_VERTEX_BUF, obj_t), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA},
      { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, offsetof(OBJ_VERTEX_BUF, obj_c), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA}
    };


    for (int i = 0; i < m_shaderCount; i++)
    {
        // パイプラインステートオブジェクトの生成.
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

        psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsList[i].Get());
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psList[i].Get());

        // ブレンドステート設定
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        // ラスタライザーステート
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        // 出力先は1ターゲット
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        // デプスバッファのフォーマットを設定
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

        psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };

        // ルートシグネチャのセット
        psoDesc.pRootSignature = m_rootSignature.Get();
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        // マルチサンプル設定
        psoDesc.SampleDesc = { 1,0 };
        psoDesc.SampleMask = UINT_MAX; // これを忘れると絵が出ない＆警告も出ないので注意.

        HRESULT hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline[i]));
        if (FAILED(hr))
        {
            throw std::runtime_error("CreateGraphicsPipelineState failed");
        }
    }

    return true;
}


// モデルの追加
int DirectXBase::AddModelObj(int addCount)
{
    // 先頭番号
    int headNo = m_ModelObjCount;
    // モデルの追加
    m_ModelObjCount += addCount;
    m_ModelObj.resize(m_ModelObjCount);

    // 定数バッファの再設定
    PrepareDescriptorHeapFor();

    return headNo;
}

// モデルの設定
// mNo：モデルの番号
// tNo：テクスチャ番号
// sNo：シェーダー番号
// bNo：バーテックスバッファ・インデックスバッファ番号
int DirectXBase::InitModelObj(int mNo, int tNo, int sNo, int bNo)
{
    // エラーチェック
    if (m_ModelObjCount <= mNo || mNo < 0)
    {
        OutputDebugStringW( L"モデル初期化エラー" );
        return false;
    }

    m_ModelObj[mNo].constantNo = mNo;
    m_ModelObj[mNo].textureNo = tNo;
    m_ModelObj[mNo].shaderNo = sNo;
    m_ModelObj[mNo].bufferNo = bNo;
}


void DirectXBase::SetModelObjDisp(int mNo, bool flag)
{
    // エラーチェック
    if (m_ModelObjCount <= mNo || mNo < 0)
    {
        OutputDebugStringW(L"モデル初期化エラー");
        return;
    }

    m_ModelObj[mNo].dispFlag = flag;
}


void DirectXBase::SetModelObjOff()
{
    // エラーチェック
    if (m_ModelObjCount < 1)
    {
        OutputDebugStringW(L"モデル初期化エラー");
        return;
    }

    for (int i = 0; i < m_ModelObjCount; i++)
    {
        m_ModelObj[i].dispFlag = false;
    }
}