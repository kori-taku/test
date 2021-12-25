#pragma once
#include <iostream>

#include "common/D3D12AppBase.h"
#include <DirectXMath.h>
#include "common/ModelObj.h"


// 前方宣言
class ModelData;


class DirectXBase : public D3D12AppBase
{
public:
    DirectXBase() : D3D12AppBase() { }

    //void PrepareInit();

    virtual void Prepare() override;
    virtual void Cleanup() override;
    virtual void MakeCommand(ComPtr<ID3D12GraphicsCommandList>& command) override;

    void PrepareInit();


    struct Vertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT4 Color;
        DirectX::XMFLOAT2 UV;
    };

    struct ShaderParameters
    {
        DirectX::XMFLOAT4X4 mtxWorld;
        DirectX::XMFLOAT4X4 mtxView;
        DirectX::XMFLOAT4X4 mtxProj;
    };

    enum
    {
        TextureSrvDescriptorBase = 0,
        ConstantBufferDescriptorBase = 0,
        // サンプラーは別ヒープなので先頭を使用
        SamplerDescriptorBase = 0,
    };

    // テクスチャファイルセット
    int SetTextureFile(const std::string& texName);
    // テクスチャセット
    bool SetTexture();


    // バーテックスバッファ・インデックスバッファセット
    int SetModel(ModelData* model);
    // バーテックスバッファ・インデックスバッファビューセット
    bool SetBufferView(int bufferNo);


    // シェーダーファイル設定
    int SetShaderFile(const std::wstring& vsName, const std::wstring& psName);
    // シェーダーセット
    bool SetShader();


    // モデルオブジェクト設定
    int AddModelObj(int addCount = 1);
    int InitModelObj(int mNo, int tNo, int sNo, int bNo);
    void SetModelObjDisp(int mNo, bool flag);
    void SetModelObjOff();



private:
    ComPtr<ID3D12Resource1> CreateBuffer(UINT bufferSize, const void* initialData);
    ComPtr<ID3D12Resource1> CreateTexture(const std::string& fileName);
    void PrepareDescriptorHeapFor();

    ComPtr<ID3D12DescriptorHeap> m_heapSrvCbv;
    ComPtr<ID3D12DescriptorHeap> m_heapSampler;
    UINT  m_samplerDescriptorSize;


    int m_bufferCount = 0;
    std::vector<ComPtr<ID3D12Resource1>> m_vbufferList;
    std::vector<int> m_vSizeList;
    std::vector<ComPtr<ID3D12Resource1>> m_ibufferList;
    std::vector<int> m_iSizeList;

    int m_textureCount = 0;
    std::vector<ComPtr<ID3D12Resource1>> m_textureList;

    ComPtr<ID3D12Resource1> m_vertexBuffer;
    ComPtr<ID3D12Resource1> m_indexBuffer;
    ComPtr<ID3D12Resource1> m_texture;
    D3D12_VERTEX_BUFFER_VIEW  m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW   m_indexBufferView;
    UINT  m_indexCount;


    int m_shaderCount = 0;
    std::vector<ComPtr<ID3DBlob>> m_vsList;
    std::vector<ComPtr<ID3DBlob>> m_psList;

    //ComPtr<ID3DBlob>  m_vs, m_ps;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    std::vector<ComPtr<ID3D12PipelineState>> m_pipeline;
    std::vector<ComPtr<ID3D12Resource1>> m_constantBuffers;

    D3D12_GPU_DESCRIPTOR_HANDLE                 m_sampler;
    std::vector < D3D12_GPU_DESCRIPTOR_HANDLE>  m_srv;
    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>    m_cbViews;

    // 表示するオブジェクトの数
    int m_ModelObjCount = 0;
    std::vector<ModelObj> m_ModelObj;

};