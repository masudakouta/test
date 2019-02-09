//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : ���c����
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_TITLE		"data/MODEL/title.x"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE	"data/TEXTURE/title.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_LOGO_START	"data/TEXTURE/coollogo_com-3677941.png"

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l
#define	POS_CAMERA_P_X	(0.0f)											// �J�������_�̏����ʒu(X���W)
#define	POS_CAMERA_P_Y	(30.0f)											// �J�������_�̏����ʒu(Y���W)
#define	POS_CAMERA_P_Z	(-50.0f)										// �J�������_�̏����ʒu(Z���W)

//#define	TITLE_LOGO_POS_X		(220)		// �^�C�g�����S�̈ʒu(X���W)
//#define	TITLE_LOGO_POS_Y		(40)		// �^�C�g�����S�̈ʒu(Y���W)
//#define	TITLE_LOGO_WIDTH		(940)		// �^�C�g�����S�̕�
//#define	TITLE_LOGO_HEIGHT		(640)		// �^�C�g�����S�̍���

#define	START_POS_X				(400)		// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y				(720)		// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH				(480)		// �X�^�[�g�{�^���̕�
#define	START_HEIGHT			(120)		// �X�^�[�g�{�^���̍���

#define	COUNT_APPERA_START		(60)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_START		(60)		// �X�^�[�g�{�^���_�ł̎���

#define	COUNT_WAIT_DEMO			(60 * 5)	// �f���܂ł̑҂�����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
//void SetColorTitleLogo(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int						g_nCountAppearStart = 0;		//
float					g_fAlphaLogo = 0.0f;			//
int						g_nCountDisp = 0;				//
bool					g_bDispStart = false;			//
int						g_nConutDemo = 0;				//

LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
//LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPD3DXMESH				g_pD3DXMeshTitle;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER			g_pD3DXMatBuffTitle;			// ���b�V���̃}�e���A�������i�[
DWORD					g_nNumMatTitle;					// �������̑���

D3DXMATRIX				g_mtxWorldTitle;				// ���[���h�}�g���b�N�X

D3DXVECTOR3			posTitle;							// ���f���̈ʒu
D3DXVECTOR3			rotTitle;							// ���f���̌���(��])
D3DXVECTOR3			sclTitle;							// ���f���̑傫��(�X�P�[��)

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearStart = 0;
	g_fAlphaLogo = 0.0f;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_nConutDemo = 0;

	// �@���𐳋K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	posTitle = D3DXVECTOR3(0.0f, 90.0f, -100.0f);
	rotTitle = D3DXVECTOR3(0.0f, -20.0f, -0.5f);
	sclTitle = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	g_pD3DTextureTitle = NULL;
	g_pD3DXMeshTitle = NULL;
	g_pD3DXMatBuffTitle = NULL;
	g_nNumMatTitle = 0;

	// ���_���̍쐬
	MakeVertexTitle(pDevice);


	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_LOGO_START,			// �t�@�C���̖��O
								&g_pD3DTextureStart);		// �ǂݍ��ރ������[
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX/*�����������Ă����*/(MODEL_TITLE,		// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
		pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pD3DXMatBuffTitle,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^�@�ގ����
		NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMatTitle,						// D3DXMATERIAL�\���̂̐��@���̃}�e���������邩
		&g_pD3DXMeshTitle)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�@
	{
		return E_FAIL;
	}
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_TITLE,			// �t�@�C���̖��O
								&g_pD3DTextureTitle);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
//	if(g_pD3DTextureTitle != NULL)
//	{// �e�N�X�`���̊J��
//		g_pD3DTextureTitle->Release();
//		g_pD3DTextureTitle = NULL;
//	}
//
//	if(g_pD3DVtxBuffTitle != NULL)
//	{// ���_�o�b�t�@�̊J��
//		g_pD3DVtxBuffTitle->Release();
//		g_pD3DVtxBuffTitle = NULL;
//	}
//
	//if(g_pD3DTextureTitleLogo != NULL)
	//{// �e�N�X�`���̊J��
	//	g_pD3DTextureTitleLogo->Release();
	//	g_pD3DTextureTitleLogo = NULL;
	//}

	//if(g_pD3DVtxBuffTitleLogo != NULL)
	//{// ���_�o�b�t�@�̊J��
	//	g_pD3DVtxBuffTitleLogo->Release();
	//	g_pD3DVtxBuffTitleLogo = NULL;
//	}
//
	if(g_pD3DTextureStart != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if(g_pD3DVtxBuffStart != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}

	if (g_pD3DTextureTitle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if (g_pD3DXMeshTitle != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshTitle->Release();
		g_pD3DXMeshTitle = NULL;
	}

	if (g_pD3DXMatBuffTitle != NULL)
	{// �}�e���A���̊J��
		g_pD3DXMatBuffTitle->Release();
		g_pD3DXMatBuffTitle = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
#if 0
	if(g_nCountAppearStart >= COUNT_APPERA_START)
	{
		g_nConutDemo++;
		if(g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
#endif
	D3DXVECTOR3 rotCamera;

	rotCamera = GetRotCamera();


	if(g_fAlphaLogo < 1.0f)
	{
		g_fAlphaLogo += 0.005f;
		if(g_fAlphaLogo >= 1.0f)
		{
			g_fAlphaLogo = 1.0f;
		}
		//SetColorTitleLogo();
	}
	else
	{
		g_nCountAppearStart++;
		if(g_nCountAppearStart > COUNT_APPERA_START)
		{
			g_nCountDisp = (g_nCountDisp + 1) % 80;
			if(g_nCountDisp > INTERVAL_DISP_START)
			{
				g_bDispStart = false;
			}
			else
			{
				g_bDispStart = true;
			}
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{
		if(g_nCountAppearStart == 0)
		{// �^�C�g���o��X�L�b�v
			g_fAlphaLogo = 1.0f;
			//SetColorTitleLogo();

			g_nCountAppearStart = COUNT_APPERA_START;
		}
		else
		{// �Q�[����
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXMATRIX mtxRot, mtxTranslate;
	//D3DXMATERIAL *pD3DXMat;


	//// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
 //   pDevice->SetStreamSource(0, g_pD3DVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	//// ���_�t�H�[�}�b�g�̐ݒ�
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// �e�N�X�`���̐ݒ�
	//pDevice->SetTexture(0, g_pD3DTextureTitleLogo);

	//// �|���S���̕`��
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	//if(g_bDispStart == true)
	//{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	

	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;
		Device->SetStreamSource(0, g_pD3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, g_pD3DTextureStart);

		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldTitle);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, sclTitle.x, sclTitle.y, sclTitle.z);
	//D3DXMatrixScaling(&mtxScl, 0.1f,0.1f,0.1f);

	D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTitle.y, rotTitle.x, rotTitle.z);
	D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posTitle.x, posTitle.y, posTitle.z);
	D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &g_mtxWorldTitle);

	// ���݂̃}�e���A�����擾//���̖߂����߂̃o�b�N�A�b�v
	Device->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	D3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffTitle->GetBufferPointer();

	//�}�e���A���̐�������
	for (int nCntMat = 0; nCntMat < (int)g_nNumMatTitle; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		Device->SetMaterial(&D3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, g_pD3DTextureTitle);

		// �`��
		g_pD3DXMeshTitle->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	Device->SetMaterial(&matDef);



	//// ���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&g_mtxWorldTitle);

	//// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTitle.y, rotTitle.x, rotTitle.z);
	//D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxRot);

	//// �ړ��𔽉f
	//D3DXMatrixTranslation(&mtxTranslate, posTitle.x, posTitle.y, posTitle.z);
	//D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxTranslate);

	//// ���[���h�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldTitle);

	//// �}�e���A�����ɑ΂���|�C���^���擾
	//pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffTitle->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_nNumMatTitle; nCntMat++)
	//{
	//	// �}�e���A���̐ݒ�
	//	pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, g_pD3DTextureTitle);

	//	// �`��
	//	g_pD3DXMeshTitle->DrawSubset(nCntMat);
	//}

	//{// �}�e���A�����f�t�H���g�ɖ߂�
	//	D3DXMATERIAL mat;

	//	mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//	mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//	mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//	pDevice->SetMaterial(&mat.MatD3D);
	//}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
 //   if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
	//											D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
	//											FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
	//											D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
	//											&g_pD3DVtxBuffTitle,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	//											NULL)))						// NULL�ɐݒ�
	//{
 //       return E_FAIL;
	//}
//
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_2D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
//
//		// ���_���W�̐ݒ�
//		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
//
//		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
//		pVtx[0].rhw =
//		pVtx[1].rhw =
//		pVtx[2].rhw =
//		pVtx[3].rhw = 1.0f;
//
//		// ���ˌ��̐ݒ�
//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		// �e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffTitle->Unlock();
//	}
//
//	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
//    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
//												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
//												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
//												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
//												&g_pD3DVtxBuffTitleLogo,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//												NULL)))						// NULL�ɐݒ�
//	{
//        return E_FAIL;
//	}
//
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_2D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);
//
//		// ���_���W�̐ݒ�
//		pVtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
//
//		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
//		pVtx[0].rhw =
//		pVtx[1].rhw =
//		pVtx[2].rhw =
//		pVtx[3].rhw = 1.0f;
//
//		// ���ˌ��̐ݒ�
//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//
//		// �e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffTitleLogo->Unlock();
//	}
//
//
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffStart,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}
//
	//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffStart->Unlock();
	

	return S_OK;
}
//
////=============================================================================
//// ���_�̍쐬
////=============================================================================
//void SetColorTitleLogo(void)
//{
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_2D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);
//
//		// ���ˌ��̐ݒ�
//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffTitleLogo->Unlock();
//	}
//
//}
//
