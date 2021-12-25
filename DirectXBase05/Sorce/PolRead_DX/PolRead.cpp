//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//  PoligonModel �ǂݍ���
//----------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

#include "PolRead.h"



char    *f_pol = NULL;      // POL �t�@�C���f�[�^

OBJ_VARIABLE    *obj;       // �C���f�b�N�X�^ �|���S���I�u�W�F�N�g
OBJ_VERTEX_BUF  *ver_buf;   // �V�[�N�G���X�^ �|���S���I�u�W�F�N�g
OBJ_VERTEX_BUF2 *ver_buf2;  // �V�[�N�G���X�^ �|���S���I�u�W�F�N�g


int OBJ_ERREND1( void );
int OBJ_ERREND2( void );
int OBJ_ERREND3( void );


//----------------------------------------------------------------------
//  LoadPolFile    POL�t�@�C���ǂݍ��ݏ���
//  �`��
//      OBJ_VARIABLE * LoadPolFile( char *f_name )
//  ����
//      f_name      �t�@�C����
//      r           �f�[�^�o�b�t�@
//  �Ԓl
//      int         ����:TRUE
//                  �ُ�:FALSE
//  ���
//     �t�@�C���T�C�Y�𒲂ׂăf�[�^��ǂݍ��ޗ̈���m�ۂ��܂��B
//     �m�ۂ��ꂽ�̈�Ƀt�@�C������S�f�[�^��ǂݍ��݂܂��B
//----------------------------------------------------------------------
OBJ_VARIABLE * LoadPolFile(const char *f_name )
{
    int     ret =TRUE;          // ����
    int     p_size =0;          // POL �t�@�C���T�C�Y

    // �|�C���^������
    f_pol   = NULL;
    obj     = NULL;
    ver_buf = NULL;
    ver_buf2= NULL;

    // �t�@�C���ǂݍ���
    // �|���S���f�[�^�ǂݍ���
    f_pol = LoadTexFile( f_name, &p_size );
    // �f�[�^�̈�̊m�ۃ`�F�b�N
    if( f_pol == NULL )
    {
        OBJ_ERREND1();
        return FALSE;
    }

    // �|���S���f�[�^�̈�m��
    obj = new OBJ_VARIABLE;
    if( obj == NULL )
    {
        OBJ_ERREND1();
        return FALSE;
    }
    memset( obj, 0x00, sizeof(OBJ_VARIABLE) );

    // �e�L�X�g���琔�l�ɕϊ�
    ret = ObjPoly( f_pol, p_size, obj );
    if( ret == NULL )
    {
        OBJ_ERREND1();
        return FALSE;
    }

    // �e�L�X�g�̈�J��
    delete [] f_pol;

    return obj;
}


//----------------------------------------------------------------------
//  LoadTexFile    �t�@�C���̑S�f�[�^�ǂݍ���
//  �`��
//      char * LoadTexFile( char *fname, int *fsize )
//  ����
//      fname       �t�@�C����
//      buf         �f�[�^�o�b�t�@
//  �Ԓl
//      int         ����ł���΃t�@�C���T�C�Y��Ԃ��܂��B
//  ���
//     �t�@�C���T�C�Y�𒲂ׂăf�[�^��ǂݍ��ޗ̈���m�ۂ��܂��B
//     �m�ۂ��ꂽ�̈�Ƀt�@�C������S�f�[�^��ǂݍ��݂܂��B
//----------------------------------------------------------------------
char * LoadTexFile(const char *fname, int *fsize )
{
    FILE    *fp;
    size_t  size;
    char    *buf;
    int     loop;

    obj     = NULL;
    ver_buf = NULL;

    fp = fopen( fname, "rt" );                      // �t�@�C�����J��
    if( fp == NULL )    return FALSE;
    fseek( fp, 0, SEEK_END );                       // �t�@�C���T�C�Y�擾
    size = ftell( fp );

    buf = new char[size];                           // �o�b�t�@�m��
    if( buf == NULL )
    {
        fclose( fp );
       return FALSE;
    }

    fseek( fp, 0, SEEK_SET );                       // �t�@�C���̐擪�ɖ߂�
    size = fread( buf, sizeof(char), size, fp );    // �S�f�[�^���o�b�t�@�ɓǂݍ���
    fclose( fp );                                   // �t�@�C�������

    *fsize = (int)size;

    // �e�L�X�g�t�@�C���̉��s�𕶎���I���R�[�h�ɕϊ�
    for( loop=0; loop<*fsize; loop++ )
    {
        if( *(buf+loop) == 0x0A )
        {
            *(buf+loop) = 0x00;
        }
    }

    return buf;
}


//----------------------------------------------------------------------
//  ObjPoly     POL�f�[�^�ϊ�
//  �`��
//      int ObjPoly( char *buf, int size, Obj_VARIABLE *r )
//  ����
//      buf     �f�[�^�o�b�t�@
//      r       �t�@�C����
//  �Ԓl
//      int     ����:TRUE
//              �ُ�:FALSE
//  ���
//     ���_�����̃f�[�^�̈���m�ۂ��܂��B
//     �m�ۂ��ꂽ�̈�Ƀf�[�^���Z�b�g���܂��B
//----------------------------------------------------------------------
int ObjPoly( char *buf, int size, OBJ_VARIABLE *r )
{
    int     loop;
    int     ver_cnt;    // ���_��
    int     nor_cnt;    // �@����
    int     col_cnt;    // �J���[��
    int     tex_cnt;    // �e�N�X�`�����W��
    int     pol_cnt;    // �|���S����
    int     point;      // �f�[�^�|�C���g

    XMFLOAT3    *vp;    // ���_�f�[�^�p�|�C���^
    XMFLOAT3    *np;    // �@���f�[�^�p�|�C���^
    XMFLOAT4    *cp;    // �J���[�f�[�^�p�|�C���^
    XMFLOAT2    *tp;    // �e�N�X�`���f�[�^�p�|�C���^
    OBJ_POLY    *pp;    // �|���S���f�[�^�p�|�C���^

    // �f�[�^�J�n�s�̌���
    point = BufChk("# Data Count", buf, size );
    if( point == FALSE ) return FALSE;
    // ���_���F�@�����F�J���[���F�|���S����
    sscanf( (buf+point), "%d %d %d %d %d", &ver_cnt, &nor_cnt, &col_cnt, &tex_cnt, &pol_cnt );


    // �f�[�^���ݒ�
    r->tex_cnt = tex_cnt;   // �e�N�X�`�����W��
    r->col_cnt = col_cnt;   // �J���[��
    r->nol_cnt = nor_cnt;   // �@����
    r->ver_cnt = ver_cnt;   // ���_���W��
    r->pol_cnt = pol_cnt;   // �|���S����


    // �f�[�^�̈�m��
    r->obj_pol = new OBJ_POLY[pol_cnt];
    r->obj_v   = new XMFLOAT3[ver_cnt];
    r->obj_n   = new XMFLOAT3[nor_cnt];
    r->obj_c   = new XMFLOAT4[col_cnt];
    r->obj_t   = new XMFLOAT2[tex_cnt];

    if( (r->obj_pol == NULL)
     || (r->obj_v   == NULL)
     || (r->obj_n   == NULL)
     || (r->obj_c   == NULL)
     || (r->obj_t   == NULL) )  return FALSE;

    // ���_�f�[�^�J�n�s����
    point = BufChk( "# Vertex Data", buf, size );
    if( point == FALSE ) return FALSE;
    // ���_�f�[�^�Z�b�g */
    for( loop=0; loop<ver_cnt; loop++ )
    {
        vp = (r->obj_v)+loop;
        sscanf( (buf+point), "%f,%f,%f", &vp->x, &vp->y, &vp->z );
        point += (int)strlen( (buf+point) ) + 1;        // �P�s���ړ�
    }

    // �@���f�[�^�J�n�s����
    point = BufChk( "# Normal Data", buf, size );
    if( point == FALSE ) return FALSE;
    // �@���f�[�^�Z�b�g
    for( loop=0; loop<nor_cnt; loop++ )
    {
        np = (r->obj_n)+loop;
        sscanf( (buf+point), "%f,%f,%f", &np->x, &np->y, &np->z );
        point += (int)strlen( (buf+point) ) + 1;        // �P�s���ړ�
    }

    // �J���[�f�[�^�J�n�s����
    point = BufChk( "# Color Data", buf, size );
    if( point == FALSE ) return FALSE;
    // �J���[�f�[�^�Z�b�g
    for( loop=0; loop<col_cnt; loop++ )
    {
        cp = (r->obj_c)+loop;
        sscanf( (buf+point), "%f,%f,%f,%f", &cp->x, &cp->y, &cp->z, &cp->w );
        point += (int)strlen( (buf+point) ) + 1;        // �P�s���ړ�
    }

    // �e�N�X�`���f�[�^�J�n�s����
    point = BufChk( "# Texture Data", buf, size );
    if( point == FALSE ) return FALSE;
    // �e�N�X�`���f�[�^�Z�b�g
    for( loop=0; loop<tex_cnt; loop++ )
    {
        tp = (r->obj_t)+loop;
        sscanf( (buf+point), "%f,%f", &tp->x, &tp->y );
        point += (int)strlen( (buf+point) ) + 1;        // �P�s���ړ�
    }

    // �|���S���f�[�^�J�n�s����
    point = BufChk( "# Polygon Data", buf, size );
    if( point == FALSE ) return FALSE;
    // �|���S���f�[�^�Z�b�g
    for( loop=0; loop<pol_cnt; loop++ )
    {
        pp = (r->obj_pol)+loop;
        sscanf( (buf+point), "%d,%d,%d : %d,%d,%d : %d,%d,%d : %d,%d,%d",
                        &pp->v[0], &pp->v[1], &pp->v[2],    // ���_�C���f�b�N�X
                        &pp->n[0], &pp->n[1], &pp->n[2],    // �@���C���f�b�N�X
                        &pp->c[0], &pp->c[1], &pp->c[2],    // �J���[�C���f�b�N�X
                        &pp->t[0], &pp->t[1], &pp->t[2] );  // �e�N�X�`���C���f�b�N�X
        point += (int)strlen( (buf+point) ) + 1;        // �P�s���ړ�
    }

    return TRUE;
}


//----------------------------------------------------------------------
//  BufChk      �f�[�^����
//  �`��
//      int     BufChk( char *chk, char *buf, int size )
//  ����
//      chk     �`�F�b�N�f�[�^
//      buf     �f�[�^�o�b�t�@
//      size    �f�[�^�o�b�t�@�̃T�C�Y
//  �Ԓl
//      int     ����:�f�[�^�̌��������ʒu��Ԃ��܂��B
//              �ُ�:FALSE
//  ���
//     �w�肳�ꂽ�f�[�^���o�b�t�@���猟�����܂��B
//----------------------------------------------------------------------
int BufChk( const char *chk, char *buf, int size )
{
    int     leng;                       // �P�s�̕�����
    int     point=0;                    // �ړ��|�C���^
    int     chk_size;                   // �`�F�b�N������

    chk_size = (int)strlen( chk );      // �`�F�b�N������

    // �o�b�t�@������
    while( point<size )
    {
        leng = (int)strlen(buf) + 1;                // �P�s�̕�����
        point += leng;                              // �o�b�t�@�ʒu�ړ�
        if( memcmp( chk, buf, chk_size ) == 0 )     // ��������r����
        {
            return point;                           // �o�b�t�@�ʒu��Ԃ�
        }
        buf += leng;                                // �o�b�t�@���P�s���i�߂�
    }

    return FALSE;                                   // �f�[�^����
}




//----------------------------------------------------------------------
//  VertexBufSet2 ���_�o�b�t�@�p�f�[�^�̈�փR�s�[
//  �`��
//      OBJ_VERTEX_BUF2 * VertexBufSet2( OBJ_VARIABLE *obj_dat )
//  ����
//  �Ԓl
//  ���
//----------------------------------------------------------------------
OBJ_VERTEX_BUF2 * VertexBufSet2( OBJ_VARIABLE *obj_dat )
{
    XMFLOAT2    tex_d = { 0.0f, 0.0f };
    XMFLOAT4    col_d = { 1.0f, 1.0f, 1.0f, 1.0f };
    XMFLOAT3    nol_d = { 1.0f, 0.0f, 0.0f };
    XMFLOAT3    ver_d = { 0.0f, 0.0f, 0.0f };

    int     loop0, loop1;
    int     dat_cnt;
    int     index_no;

    OBJ_POLY    *obj_pol;             // �|���S���C���f�b�N�X �o�b�t�@

    OBJ_VERTEX_BUF  *obj_buf;

    XMFLOAT2    *obj_t1;     // �e�N�X�`���o�b�t�@
    XMFLOAT4    *obj_c1;     // �J���[�o�b�t�@
    XMFLOAT3    *obj_n1;     // �@���o�b�t�@
    XMFLOAT3    *obj_v1;     // ���_�o�b�t�@

    int tex_f = FALSE;     // �e�N�X�`�����W�t���O
    int col_f = FALSE;     // �J���[�t���O
    int nol_f = FALSE;     // �@���t���O
    int ver_f = FALSE;     // ���_���W�t���O

    // �|�C���^������
    obj     = NULL;
    ver_buf = NULL;
    ver_buf2= NULL;

    // �|���S���V�[�N�G���X �o�b�t�@
    ver_buf2 = new OBJ_VERTEX_BUF2;
    memset( ver_buf2, 0x00, sizeof(OBJ_VERTEX_BUF2) );

    // ���_���v�Z
    dat_cnt = obj_dat->pol_cnt * 3;

    ver_buf2->pol_cnt  = obj_dat->pol_cnt;                      // �|���S����
    ver_buf2->dat_cnt  = dat_cnt;                               // �f�[�^���i���_���j


    // ���_�f�[�^�̈�m��
    ver_buf2->obj_index = new unsigned int [dat_cnt];
    ver_buf2->obj_dat   = new OBJ_VERTEX_BUF[dat_cnt];


    obj_buf = ver_buf2->obj_dat;// �R�s�[��o�b�t�@
    obj_t1  = obj_dat->obj_t;   // �R�s�[���o�b�t�@
    obj_c1  = obj_dat->obj_c;   // �R�s�[���o�b�t�@
    obj_n1  = obj_dat->obj_n;   // �R�s�[���o�b�t�@
    obj_v1  = obj_dat->obj_v;   // �R�s�[���o�b�t�@
    obj_pol = obj_dat->obj_pol; // �|���S���f�[�^�[�擪�A�h���X


    if (obj_dat->tex_cnt > 0)  tex_f = TRUE; // �e�N�X�`�����W�t���O
    if (obj_dat->col_cnt > 0)  col_f = TRUE; // �J���[�t���O
    if (obj_dat->nol_cnt > 0)  nol_f = TRUE; // �@���t���O
    if (obj_dat->ver_cnt > 0)  ver_f = TRUE; // ���_���W�t���O


    int dataCount=0;

    // �|���S���f�[�^��������
    for( loop0=0; loop0<obj_dat->pol_cnt; loop0++ )
    {

        // �|���S�����_�f�[�^��������
        for( loop1=0; loop1<3; loop1++ )
        {

            // �C���f�b�N�X�Z�b�g
            ver_buf2->obj_index[dataCount] = dataCount;
            
            OBJ_VERTEX_BUF* obj_dat = &ver_buf2->obj_dat[dataCount];

            // �e�N�X�`�����W�f�[�^�R�s�[
            if( tex_f == TRUE )
            {
                index_no = obj_pol->t[loop1];           // �C���f�b�N�X�ԍ����o��
                obj_dat->obj_t = *(obj_t1 + index_no);  // �f�[�^�R�s�|
            }
            else
            {
                obj_dat->obj_t = tex_d;                 // �_�~�[�f�[�^�R�s�[
            }


            // �J���[�f�[�^�R�s�[
            if( col_f == TRUE )
            {
                index_no = obj_pol->c[loop1];           // �C���f�b�N�X�ԍ����o��
                obj_dat->obj_c = *(obj_c1 + index_no);  // �f�[�^�R�s�[
            }
            else
            {
                obj_dat->obj_c = col_d;                 // �_�~�[�f�[�^�R�s�[
            }


            // �@���f�[�^�R�s�[
            if( nol_f == TRUE )
            {
                index_no = obj_pol->n[loop1];           // �C���f�b�N�X�ԍ����o��
                obj_dat->obj_n = *(obj_n1 + index_no);  // �f�[�^�R�s�|
            }
            else
            {
                obj_dat->obj_n = nol_d;                 // �_�~�[�f�[�^�R�s�[
            }


            // ���_���W�f�[�^�R�s�[
            if( ver_f == TRUE )
            {
                index_no = obj_pol->v[loop1];           // �C���f�b�N�X�ԍ����o��
                obj_dat->obj_v = *(obj_v1 + index_no);  // �f�[�^�R�s�|
            }
            else
            {
                obj_dat->obj_v = ver_d;                 // �_�~�[�f�[�^�R�s�[
            }

            // �f�[�^�o�b�t�@�X�V
            dataCount++;
        }

        obj_pol++;
    }

    return ver_buf2;
}


//----------------------------------------------------------------------
//  �f�[�^�[�J������ (OBJ_VARIABLE)
//----------------------------------------------------------------------
void ObjIndexFree( OBJ_VARIABLE *objp )
{
    if( objp != NULL )
    {
        if( objp->obj_v   != NULL )
        {
            delete[] objp->obj_v;
            objp->obj_v = NULL;
        }

        if( objp->obj_n   != NULL )
        {
            delete[] objp->obj_n;
            objp->obj_n = NULL;
        }

        if( objp->obj_c   != NULL )
        {
            delete[] objp->obj_c;
            objp->obj_c = NULL;
        }

        if( objp->obj_t   != NULL )
        {
            delete[] objp->obj_t;
            objp->obj_t = NULL;
        }

        if( objp->obj_pol != NULL )
        {
            delete[] objp->obj_pol;
            objp->obj_pol = NULL;
        }
        delete objp;
    }
}


//----------------------------------------------------------------------
//  �f�[�^�[�J������ (OBJ_VERTEX_BUF)
//----------------------------------------------------------------------
void ObjVertexFree( OBJ_VERTEX_BUF *objp )
{
    if( objp != NULL )
    {
        delete objp;
    }
}


//----------------------------------------------------------------------
//  �f�[�^�[�J������ (OBJ_VERTEX_BU2F)
//----------------------------------------------------------------------
void ObjVertexFree2( OBJ_VERTEX_BUF2 *objp )
{
    if( objp != NULL )
    {
        if (objp->obj_index != NULL)
        {
            delete[] objp->obj_index;
            objp->obj_index = NULL;
        }        
        
        if (objp->obj_dat != NULL)
        {
            delete[] objp->obj_dat;
            objp->obj_dat = NULL;
        }

        delete objp;
    }
}


//----------------------------------------------------------------------
//  �G���[����
//----------------------------------------------------------------------
int OBJ_ERREND1( void )
{

//    MessageBox( NULL, "OBJ �t�@�C���ǂݍ��ݎ��s", "TGA", MB_OK );

    // �|���S���f�[�^�Ǘ��|�C���^������
    if( f_pol != NULL )
    {
        delete[] f_pol;
        f_pol = NULL;
    }

    ObjIndexFree( obj );
    obj = NULL;

    ObjVertexFree2( ver_buf2 );
    ver_buf2 = NULL;

    return TRUE;
}


//----------------------------------------------------------------------
//  �G���[����
//----------------------------------------------------------------------
int OBJ_ERREND2( void )
{

//    MessageBox( NULL, "OBJ �f�[�^�ϊ����s", "TGA", MB_OK );

    // �|���S���f�[�^�Ǘ��|�C���^������
    ObjVertexFree( ver_buf );
    ver_buf = NULL;

    return TRUE;
}


//----------------------------------------------------------------------
//  �G���[����
//----------------------------------------------------------------------
int OBJ_ERREND3( void )
{

//    MessageBox( NULL, "OBJ �f�[�^�ϊ����s", "TGA", MB_OK );

    // �|���S���f�[�^�Ǘ��|�C���^������
    ObjVertexFree2( ver_buf2 );
    ver_buf2 = NULL;

    return TRUE;
}


//----------------------------------------------------------------------
//  POL�f�[�^�ǂݍ��ݏ���
//  filename : �t�@�C����
//----------------------------------------------------------------------
ModelData * ModelDataLoad(const char * filename)
{
    // ���f���f�[�^�Ǘ�
    ModelData* modelData = new ModelData();


    // POL �f�[�^�ǂݍ���
    obj = LoadPolFile(filename);
    if (obj == NULL)
    {
        return NULL;
    }

    // ���_�o�b�t�@�p�f�[�^�ϊ�
    ver_buf2 = VertexBufSet2(obj);
    if (ver_buf2 == NULL)
    {
        ObjIndexFree(obj);
        obj = NULL;
        return NULL;
    }

    // �C���f�b�N�X�^�̈�J��
    ObjIndexFree(obj);
    obj = NULL;


    // ���f���f�[�^�̈�m��
    modelData = new ModelData;
    if (modelData == NULL)
    {
        free(ver_buf2);
        ver_buf = NULL;
        return NULL;
    }

    // ���_�f�[�^�ݒ�
    modelData->vertexObj = ver_buf2;

    return modelData;
}
