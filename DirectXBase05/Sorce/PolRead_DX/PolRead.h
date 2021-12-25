//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//  DirectX プログラム
//----------------------------------------------------------------------
#pragma once

#include "../common/d3dx12.h"
#include "../DirectxTexture/DirectXTex.h" //テクスチャ読み込み用

#pragma comment (lib, "Sorce/DirectxTexture/x64/Debug/DirectXTex.lib")


#include <DirectXMath.h>
#include <d3dcompiler.h>


using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;


// オブジェクトデータ用構造体
typedef struct OBJ_POLY{
    int   v[3];         // 頂点インデックス
    int   n[3];         // 法線インデックス
    int   c[3];         // カラーインデックス
    int   t[3];         // テクスチャインデックス
} OBJ_POLY;


typedef struct OBJ_VARIABLE {
    int          tex_cnt;   // テクスチャ座標数
    int          col_cnt;   // カラー数
    int          nol_cnt;   // 法線数
    int          ver_cnt;   // 頂点座標数
    int          pol_cnt;   // ポリゴン数
    XMFLOAT2    *obj_t;     // テクスチャバッファ
    XMFLOAT4    *obj_c;     // カラーバッファ
    XMFLOAT3    *obj_n;     // 法線バッファ
    XMFLOAT3    *obj_v;     // 頂点バッファ
    OBJ_POLY    *obj_pol;   // ポリゴン情報 バッファ

    OBJ_VARIABLE()
    {
        tex_cnt = 0;
        col_cnt = 0;
        nol_cnt = 0;
        ver_cnt = 0;
        pol_cnt = 0;
        obj_t = nullptr;
        obj_c = nullptr;
        obj_n = nullptr;
        obj_v = nullptr;
        obj_pol = nullptr;
    }
} OBJ_VARIABLE;


typedef struct OBJ_VERTEX_BUF{
    XMFLOAT3    obj_v;     // 頂点バッファ
    XMFLOAT3    obj_n;     // 法線バッファ
    XMFLOAT2    obj_t;     // テクスチャバッファ
    XMFLOAT4    obj_c;     // カラーバッファ
} OBJ_VERTEX_BUF;


typedef struct OBJ_VERTEX_BUF2{
    int              pol_cnt;   // ポリゴン数
    int              dat_cnt;   // データ数
    unsigned int    *obj_index; // インデックスデータ
    OBJ_VERTEX_BUF  *obj_dat;   // 頂点データ

    OBJ_VERTEX_BUF2()
    {
        pol_cnt = 0;
        dat_cnt = 0;
        obj_index = nullptr;
        obj_dat = nullptr;
    }
} OBJ_VERTEX_BUF2;


// 頂点配列 対応フォーマット

// Polygon Mode
typedef struct {
    XMFLOAT3    c;
    XMFLOAT3    v;
} C3F_V3F;

typedef struct {
    XMFLOAT3    n;
    XMFLOAT3    v;
} N3F_V3F;

typedef struct {
    XMFLOAT4    c;
    XMFLOAT3    n;
    XMFLOAT3    v;
} C4F_N3F_V3F;

typedef struct {
    XMFLOAT2    t;
    XMFLOAT3    v;
} T2F_V3F;

typedef struct {
    XMFLOAT4    t;
    XMFLOAT4    v;
} T4F_V4F;

typedef struct {
    XMFLOAT2    t;
    XMFLOAT3    c;
    XMFLOAT3    v;
} T2F_C3F_V3F;

typedef struct {
    XMFLOAT2    t;
    XMFLOAT3    n;
    XMFLOAT3    v;
} T2F_N3F_V3F;

typedef struct {
    XMFLOAT2    t;
    XMFLOAT4    c;
    XMFLOAT3    n;
    XMFLOAT3    v;
} T2F_C4F_N3F_V3F;

typedef struct {
    XMFLOAT4    t;
    XMFLOAT4    c;
    XMFLOAT3    n;
    XMFLOAT4    v;
} T4F_C4F_N3F_V4F;


// モデルデータクラス
class ModelData
{
public:
    // 頂点バッファNo.
    int vertexNo;

    // モデルデータ
    OBJ_VERTEX_BUF2* vertexObj;

    // テクスチャNo.
    int textureNo;

    // 平行移動
    XMFLOAT3	mov;

    // 回転
    XMFLOAT3	rot;

};

//----------------------------------------------------------------------
// プロトタイプ宣言
extern  OBJ_VARIABLE *    LoadPolFile( const char *f_name );
extern  char *            LoadTexFile( const char *fname, int *size );
extern  int               ObjPoly( char *buf, int size,  OBJ_VARIABLE *r );
extern  int               BufChk( const char *chk, char *buf, int size );
extern  OBJ_VERTEX_BUF2 * VertexBufSet2( OBJ_VARIABLE *obj_dat );

extern  void ObjIndexFree( OBJ_VARIABLE *objp );
extern  void ObjVertexFree ( OBJ_VERTEX_BUF *objp );
extern  void ObjVertexFree2( OBJ_VERTEX_BUF2 *objp );

extern  ModelData * ModelDataLoad(const char * filename);
extern  void ModelDataDisp(ModelData *model);

