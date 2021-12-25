//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//  DirectX �v���O����
//----------------------------------------------------------------------
#pragma once

#include "../common/d3dx12.h"
#include "../DirectxTexture/DirectXTex.h" //�e�N�X�`���ǂݍ��ݗp

#pragma comment (lib, "Sorce/DirectxTexture/x64/Debug/DirectXTex.lib")


#include <DirectXMath.h>
#include <d3dcompiler.h>


using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;


// �I�u�W�F�N�g�f�[�^�p�\����
typedef struct OBJ_POLY{
    int   v[3];         // ���_�C���f�b�N�X
    int   n[3];         // �@���C���f�b�N�X
    int   c[3];         // �J���[�C���f�b�N�X
    int   t[3];         // �e�N�X�`���C���f�b�N�X
} OBJ_POLY;


typedef struct OBJ_VARIABLE {
    int          tex_cnt;   // �e�N�X�`�����W��
    int          col_cnt;   // �J���[��
    int          nol_cnt;   // �@����
    int          ver_cnt;   // ���_���W��
    int          pol_cnt;   // �|���S����
    XMFLOAT2    *obj_t;     // �e�N�X�`���o�b�t�@
    XMFLOAT4    *obj_c;     // �J���[�o�b�t�@
    XMFLOAT3    *obj_n;     // �@���o�b�t�@
    XMFLOAT3    *obj_v;     // ���_�o�b�t�@
    OBJ_POLY    *obj_pol;   // �|���S����� �o�b�t�@

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
    XMFLOAT3    obj_v;     // ���_�o�b�t�@
    XMFLOAT3    obj_n;     // �@���o�b�t�@
    XMFLOAT2    obj_t;     // �e�N�X�`���o�b�t�@
    XMFLOAT4    obj_c;     // �J���[�o�b�t�@
} OBJ_VERTEX_BUF;


typedef struct OBJ_VERTEX_BUF2{
    int              pol_cnt;   // �|���S����
    int              dat_cnt;   // �f�[�^��
    unsigned int    *obj_index; // �C���f�b�N�X�f�[�^
    OBJ_VERTEX_BUF  *obj_dat;   // ���_�f�[�^

    OBJ_VERTEX_BUF2()
    {
        pol_cnt = 0;
        dat_cnt = 0;
        obj_index = nullptr;
        obj_dat = nullptr;
    }
} OBJ_VERTEX_BUF2;


// ���_�z�� �Ή��t�H�[�}�b�g

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


// ���f���f�[�^�N���X
class ModelData
{
public:
    // ���_�o�b�t�@No.
    int vertexNo;

    // ���f���f�[�^
    OBJ_VERTEX_BUF2* vertexObj;

    // �e�N�X�`��No.
    int textureNo;

    // ���s�ړ�
    XMFLOAT3	mov;

    // ��]
    XMFLOAT3	rot;

};

//----------------------------------------------------------------------
// �v���g�^�C�v�錾
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

