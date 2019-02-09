//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : ���c�@����
//
//=============================================================================
#include "box_effect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PARTICLE		"data/MODEL/Effect_Box.x"	// �ǂݍ��ރ��f���t�@�C����
#define	TEXTURE_BOX_EFFECT	"data/TEXTURE/effect_box.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_SIZE_X		(50.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(2.0f)							// �ړ����x

#define	MAX_PARTICLE			(512)						// �r���{�[�h�ő吔

#define	DISP_SHADOW				// �e�̕\��
//#undef DISP_SHADOW

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT MakeVertexBox_Effect(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexBox_Effect(int nIdxBox_Effect, float fSizeX, float fSizeY);
//void SetColorBox_Effect(int nIdxBox_Effect, D3DXCOLOR col);

int						PTAlpha;						// �A���t�@�e�X�g��臒l

bool					PTAlpaTest;						// �A���t�@�e�X�gON/OFF
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBox_Effect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				D3DXMeshBox_Effect;				// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			D3DXBuffMatBox_Effect;			// �}�e���A�����ւ̃|�C���^
DWORD					NumMatBox_Effect;				// �}�e���A�����̐�

D3DXMATRIX				g_mtxWorldBox_Effect;			// ���[���h�}�g���b�N�X


PARTICLE				g_aBox_Effect[MAX_PARTICLE];		// �p�[�e�B�N�����[�N
D3DXVECTOR3				g_posBase;						// �r���{�[�h�����ʒu
float					g_fWidthBase = 5.0f;			// ��̕�
float					g_fHeightBase = 10.0f;			// ��̍���
bool					g_bPause = false;				// �|�[�YON/OFF

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBox_Effect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	//MakeVertexBox_Effect(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_BOX_EFFECT,			// �t�@�C���̖��O
		&g_pD3DTextureBox_Effect);	// �ǂݍ��ރ������[
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX/*�����������Ă����*/(MODEL_PARTICLE,		// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
		pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatBox_Effect,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^�@�ގ����
		NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatBox_Effect,						// D3DXMATERIAL�\���̂̐��@���̃}�e���������邩
		&D3DXMeshBox_Effect)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�@
	{
		return E_FAIL;
	}

	for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++)
	{
		g_aBox_Effect[nCntBox_Effect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBox_Effect[nCntBox_Effect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBox_Effect[nCntBox_Effect].scale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
		g_aBox_Effect[nCntBox_Effect].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBox_Effect[nCntBox_Effect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBox_Effect[nCntBox_Effect].fSizeX = PARTICLE_SIZE_X;
		g_aBox_Effect[nCntBox_Effect].fSizeY = PARTICLE_SIZE_Y;
		g_aBox_Effect[nCntBox_Effect].nIdxShadow = -1;
		g_aBox_Effect[nCntBox_Effect].nLife = 0;
		g_aBox_Effect[nCntBox_Effect].bUse = false;
	}

	g_posBase = D3DXVECTOR3(6000.0f, 0.0f, 0.0f);

	PTAlpha = 0;


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBox_Effect(void)
{
	if (g_pD3DTextureBox_Effect != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBox_Effect->Release();
		g_pD3DTextureBox_Effect = NULL;
	}
	if (D3DXMeshBox_Effect != NULL)
	{// �e�N�X�`���̊J��
		D3DXMeshBox_Effect->Release();
		D3DXMeshBox_Effect = NULL;
	}
	if (D3DXBuffMatBox_Effect != NULL)
	{// �e�N�X�`���̊J��
		D3DXBuffMatBox_Effect->Release();
		D3DXBuffMatBox_Effect = NULL;
	}

	//if (g_pD3DVtxBuffBox_Effect != NULL)
	//{// ���_�o�b�t�@�̊J��
	//	g_pD3DVtxBuffBox_Effect->Release();
	//	g_pD3DVtxBuffBox_Effect = NULL;
	//}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBox_Effect(void)
{
	D3DXVECTOR3 rotCamera;
	PARTICLE *Box_Effect = g_aBox_Effect;
	// �J�����̉�]���擾
	rotCamera = GetRotCamera();
	// �A���t�@�e�X�gON/OFF
	if (GetKeyboardTrigger(DIK_Y))
	{
		PTAlpaTest = PTAlpaTest ? false : true;
	}

	//if(GetKeyboardPress(DIK_LEFT))
	//{
	//	if(GetKeyboardPress(DIK_UP))
	//	{// ���O�ړ�
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if(GetKeyboardPress(DIK_DOWN))
	//	{// ����ړ�
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// ���ړ�
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if(GetKeyboardPress(DIK_RIGHT))
	//{
	//	if(GetKeyboardPress(DIK_UP))
	//	{// �E�O�ړ�
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if(GetKeyboardPress(DIK_DOWN))
	//	{// �E��ړ�
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// �E�ړ�
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if(GetKeyboardPress(DIK_UP))
	//{// �O�ړ�
	//	g_posBase.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	//else if(GetKeyboardPress(DIK_DOWN))
	//{// ��ړ�
	//	g_posBase.x -= sinf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}

	//if (GetKeyboardPress(DIK_1))
	//{
	//	g_fWidthBase -= 0.1f;
	//	if (g_fWidthBase < 2.0f)
	//	{
	//		g_fWidthBase = 2.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_2))
	//{
	//	g_fWidthBase += 0.1f;
	//	if (g_fWidthBase > 10.0f)
	//	{
	//		g_fWidthBase = 10.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_3))
	//{
	//	g_fHeightBase -= 0.1f;
	//	if (g_fHeightBase < 5.0f)
	//	{
	//		g_fHeightBase = 5.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_4))
	//{
	//	g_fHeightBase += 0.1f;
	//	if (g_fHeightBase > 15.0f)
	//	{
	//		g_fHeightBase = 15.0f;
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	g_bPause = g_bPause ? false : true;
	//}

	if (g_bPause == false)
	{
		for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++, Box_Effect++)
		{
			if (g_aBox_Effect[nCntBox_Effect].bUse)
			{// �g�p��
				g_aBox_Effect[nCntBox_Effect].pos.x += g_aBox_Effect[nCntBox_Effect].move.x;
				g_aBox_Effect[nCntBox_Effect].pos.z += g_aBox_Effect[nCntBox_Effect].move.z;

				g_aBox_Effect[nCntBox_Effect].pos.y += g_aBox_Effect[nCntBox_Effect].move.y;
				if (g_aBox_Effect[nCntBox_Effect].pos.y <= g_aBox_Effect[nCntBox_Effect].fSizeY / 2)
				{// ���n����
					g_aBox_Effect[nCntBox_Effect].pos.y = g_aBox_Effect[nCntBox_Effect].fSizeY / 2;
					g_aBox_Effect[nCntBox_Effect].move.y = -g_aBox_Effect[nCntBox_Effect].move.y * 0.75f;
				}

				g_aBox_Effect[nCntBox_Effect].move.x += (0.0f - g_aBox_Effect[nCntBox_Effect].move.x) * 0.015f;
				g_aBox_Effect[nCntBox_Effect].move.y -= 0.25f;
				g_aBox_Effect[nCntBox_Effect].move.z += (0.0f - g_aBox_Effect[nCntBox_Effect].move.z) * 0.015f;

#ifdef DISP_SHADOW
				if (g_aBox_Effect[nCntBox_Effect].nIdxShadow != -1)
				{// �e�g�p��
					float colA;

					// �e�̈ʒu�ݒ�
					SetPositionShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow, D3DXVECTOR3(g_aBox_Effect[nCntBox_Effect].pos.x, 0.1f, g_aBox_Effect[nCntBox_Effect].pos.z));

					// �e�̃��l�ݒ�
					if (g_aBox_Effect[nCntBox_Effect].col.a > 0.0f)
					{
						colA = (300.0f - (g_aBox_Effect[nCntBox_Effect].pos.y - 9.0f)) / (300.0f / g_aBox_Effect[nCntBox_Effect].col.a);
						if (colA < 0.0f)
						{
							colA = 0.0f;
						}
					}
					else
					{
						colA = 0.0f;
					}

					// �e�̐F�̐ݒ�
					//SetColorShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow, D3DXCOLOR(0.15f, 0.15f, 0.15f, colA));
				}
#endif

				g_aBox_Effect[nCntBox_Effect].nLife--;
				if (g_aBox_Effect[nCntBox_Effect].nLife <= 0)
				{
					g_aBox_Effect[nCntBox_Effect].bUse = false;
					DeleteShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow);
					g_aBox_Effect[nCntBox_Effect].nIdxShadow = -1;
				}
				else
				{
					if (g_aBox_Effect[nCntBox_Effect].nLife <= 80)
					{
						g_aBox_Effect[nCntBox_Effect].col.r = 0.60f - (float)(80 - g_aBox_Effect[nCntBox_Effect].nLife) / 8.0f * 0.06f + 0.2f;
						g_aBox_Effect[nCntBox_Effect].col.g = 0.70f - (float)(80 - g_aBox_Effect[nCntBox_Effect].nLife) / 8.0f * 0.07f;
						g_aBox_Effect[nCntBox_Effect].col.b = 0.05f;
					}

					if (g_aBox_Effect[nCntBox_Effect].nLife <= 20)
					{
						// ���l�ݒ�
						g_aBox_Effect[nCntBox_Effect].col.a -= 0.05f;
						if (g_aBox_Effect[nCntBox_Effect].col.a < 0.0f)
						{
							g_aBox_Effect[nCntBox_Effect].col.a = 0.0f;
						}
					}

					// �F�̐ݒ�
					//SetColorBox_Effect(nCntBox_Effect, g_aBox_Effect[nCntBox_Effect].col);
				}
			}
		}

		// �p�[�e�B�N������
		//if((rand() % 2) == 0)
		{
			float fAngle, fLength;
			int nLife;
			float fSize;

			Box_Effect->pos = g_posBase;

			fAngle = (float)(rand() % 30 - 15) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

			//����
			Box_Effect->move.x = sinf(fAngle) * fLength;
			Box_Effect->move.y = rand() % 30 / 30.0f + 7.0f;
			Box_Effect->move.z = cosf(fAngle) * 0.1f;

			nLife = rand() % 100 + 10;

			fSize = (float)(rand() % 3 + 2);

			Box_Effect->pos.y = fSize / 2;

			//�{�b�N�X�G�t�F�N�g�̐ݒ�
			SetBox_Effect(Box_Effect->pos, Box_Effect->move, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBox_Effect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxView, mtxScale, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;



	for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++)
	{
		if (g_aBox_Effect[nCntBox_Effect].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldBox_Effect);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aBox_Effect[nCntBox_Effect].scale.x, g_aBox_Effect[nCntBox_Effect].scale.y, g_aBox_Effect[nCntBox_Effect].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldBox_Effect, &g_mtxWorldBox_Effect, &mtxScale);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBox_Effect[nCntBox_Effect].rot.y, g_aBox_Effect[nCntBox_Effect].rot.x, g_aBox_Effect[nCntBox_Effect].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldBox_Effect, &g_mtxWorldBox_Effect, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aBox_Effect[nCntBox_Effect].pos.x, g_aBox_Effect[nCntBox_Effect].pos.y, g_aBox_Effect[nCntBox_Effect].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBox_Effect, &g_mtxWorldBox_Effect, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBox_Effect);

			// ���݂̃}�e���A�����擾//���̖߂����߂̃o�b�N�A�b�v
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ɑ΂���|�C���^���擾
			D3DXMat = (D3DXMATERIAL*)D3DXBuffMatBox_Effect->GetBufferPointer();

			//�}�e���A���̐�������
			for (int CntMat = 0; CntMat < (int)NumMatBox_Effect; CntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&D3DXMat[CntMat].MatD3D);

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0
					, g_pD3DTextureBox_Effect);

				// �`��
				D3DXMeshBox_Effect->DrawSubset(CntMat);
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
//HRESULT MakeVertexBox_Effect(LPDIRECT3DDEVICE9 pDevice)
//{
//	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
//		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
//		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
//		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
//		&g_pD3DVtxBuffBox_Effect,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//		NULL)))										// NULL�ɐݒ�
//	{
//		return E_FAIL;
//	}
//
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_3D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffBox_Effect->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++, pVtx += 4)
//		{
//			// ���_���W�̐ݒ�
//			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
//			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
//			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
//			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
//
//			// �@���̐ݒ�
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//
//			// ���ˌ��̐ݒ�
//			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// �e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffBox_Effect->Unlock();
//	}
//
//	return S_OK;
//}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
//void SetVertexBox_Effect(int nIdxBox_Effect, float fSizeX, float fSizeY)
//{
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_3D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffBox_Effect->Lock(0, 0, (void**)&pVtx, 0);
//
//		pVtx += (nIdxBox_Effect * 4);
//
//		// ���_���W�̐ݒ�
//		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffBox_Effect->Unlock();
//	}
//}
//
//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
//void SetColorBox_Effect(int nIdxBox_Effect, D3DXCOLOR col)
//{
//	{//���_�o�b�t�@�̒��g�𖄂߂�
//		VERTEX_3D *pVtx;
//
//		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//		g_pD3DVtxBuffBox_Effect->Lock(0, 0, (void**)&pVtx, 0);
//
//		pVtx += (nIdxBox_Effect * 4);
//
//		// ���_���W�̐ݒ�
//		pVtx[0].diffuse =
//			pVtx[1].diffuse =
//			pVtx[2].diffuse =
//			pVtx[3].diffuse = col;
//
//		// ���_�f�[�^���A�����b�N����
//		g_pD3DVtxBuffBox_Effect->Unlock();
//	}
//}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetBox_Effect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxBox_Effect = -1;


	for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++)
	{
		if (!g_aBox_Effect[nCntBox_Effect].bUse)
		{
			g_aBox_Effect[nCntBox_Effect].pos = pos;
			g_aBox_Effect[nCntBox_Effect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBox_Effect[nCntBox_Effect].scale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
			g_aBox_Effect[nCntBox_Effect].move.x = move.x;
			g_aBox_Effect[nCntBox_Effect].move.y= move.y;
			g_aBox_Effect[nCntBox_Effect].move.z= move.z;
			g_aBox_Effect[nCntBox_Effect].col = col;
			g_aBox_Effect[nCntBox_Effect].fSizeX = fSizeX;
			g_aBox_Effect[nCntBox_Effect].fSizeY = fSizeY;
			g_aBox_Effect[nCntBox_Effect].nLife = nLife;
			g_aBox_Effect[nCntBox_Effect].bUse = true;


			nIdxBox_Effect = nCntBox_Effect;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			g_aBox_Effect[nCntBox_Effect].nIdxShadow = SetShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), 4.5, 4.5);		// �e�̐ݒ�
			if (g_aBox_Effect[nCntBox_Effect].nIdxShadow != -1)
			{
				SetColorShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif
			break;
			return nIdxBox_Effect;
		}
	}

}
//=============================================================================
// �G�t�F�N�g���擾
//=============================================================================
PARTICLE *GetBox_Effect(int pno)
{
	return &g_aBox_Effect[pno];
}
