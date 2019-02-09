//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "gorl.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_CAR		 "data/MODEL/gorl.x"							// �ǂݍ��ރ��f����
#define TEXTURE_FILENAME "data/MODEL/gorl.png"							// �e�N�X�`��

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l

#define	POS_CAMERA_P_X	(0.0f)											// �J�������_�̏����ʒu(X���W)
#define	POS_CAMERA_P_Y	(30.0f)											// �J�������_�̏����ʒu(Y���W)
#define	POS_CAMERA_P_Z	(-100.0f)										// �J�������_�̏����ʒu(Z���W)

#define	VALUE_MOVE		(5.0f)											// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// ��]��

#define	MOVE_MODEL		(5.0f)

#define	MAX_GORL		(1)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureGorl;				// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			D3DXMeshGorl;				// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatGorl;			// �}�e���A�����ւ̃|�C���^
DWORD				NumMatGorl;					// �}�e���A�����̐�

D3DXVECTOR3			posGorl;					// ���f���̈ʒu
D3DXVECTOR3			rotGorl;					// ���f���̌���(��])
D3DXVECTOR3			sclGorl;					// ���f���̑傫��(�X�P�[��)

D3DXMATRIX			mtxWorld;					// ���[���h�}�g���b�N�X

static int					IdxShadow;			// �eID
static float				SizeShadow;			// �e�̃T�C�Y
static D3DXCOLOR			colShadow;			// �e�̐F
bool						fggorl;
float fgRadius;									// ���a

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGorl(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �@���𐳋K��
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	posGorl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotGorl = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	sclGorl = D3DXVECTOR3(1.5f, 1.5f, 1.5f);

	posGorl.x = (float)(rand() % 40 + 1);
	posGorl.z = (float)(rand() % 40 + 1);
	fggorl = false;
	D3DTextureGorl = NULL;
	D3DXMeshGorl = NULL;
	D3DXBuffMatGorl = NULL;
	NumMatGorl = 0;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX/*�����������Ă����*/(PLAYER_CAR,		// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
		Device,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatGorl,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^�@�ގ����
		NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatGorl,						// D3DXMATERIAL�\���̂̐��@���̃}�e���������邩
		&D3DXMeshGorl)))					// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�@
	{
		return E_FAIL;
	}

#if 1
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,									// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,						// �t�@�C���̖��O
		&D3DTextureGorl);					// �ǂݍ��ރ������[
#endif



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGorl(void)
{
	if (D3DTextureGorl != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureGorl->Release();
		D3DTextureGorl = NULL;
	}

	if (D3DXMeshGorl != NULL)
	{// ���b�V���̊J��
		D3DXMeshGorl->Release();
		D3DXMeshGorl = NULL;
	}

	if (D3DXBuffMatGorl != NULL)
	{// �}�e���A���̊J��
		D3DXBuffMatGorl->Release();
		D3DXBuffMatGorl = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGorl(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	//if (pPlayer->pos.x == posGorl.x)
	//{
	//	SetFade(FADE_OUT);

	//}
	for (int nCntItem = 0; nCntItem < MAX_GORL; nCntItem++)
	{
		if (fggorl == true)
		{
			float fLength;

			fLength = (pPlayer->pos.x - posGorl.x) * (pPlayer->pos.x - posGorl.x)
				+ (pPlayer->pos.y - posGorl.y) * (pPlayer->pos.y - posGorl.y)
				+ (pPlayer->pos.z - posGorl.z) * (pPlayer->pos.z - posGorl.z);
			if (fLength < (pPlayer->fRadius + fgRadius) * (pPlayer->fRadius + fgRadius))
			{

				SetFade(FADE_OUT);

				// SE�Đ�
				//PlaySound(SOUND_LABEL_SE_COIN);
			}
		}

	}

	posGorl.y = Get(posGorl);

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGorl(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, sclGorl.x, sclGorl.y, sclGorl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotGorl.y, rotGorl.x, rotGorl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posGorl.x, posGorl.y, posGorl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾//���̖߂����߂̃o�b�N�A�b�v
	Device->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	D3DXMat = (D3DXMATERIAL*)D3DXBuffMatGorl->GetBufferPointer();

	//�}�e���A���̐�������
	for (int CntMat = 0; CntMat < (int)NumMatGorl; CntMat++)
	{
		// �}�e���A���̐ݒ�
		Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0
			, D3DTextureGorl);

		// �`��
		//D3DXMeshGorl->DrawSubset(CntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	Device->SetMaterial(&matDef);
}

//=============================================================================
// �G�l�~�[�̎擾�֐�
//=============================================================================

D3DXVECTOR3 *GetGorlPos(void)
{
	return &posGorl;
}
//=============================================================================
// �v���C���[�ƃG�l�~�[�̓����蔻��
//=============================================================================

//void CollisionDetection(void)
//{
//	for (int K = 0; K < ENEMY_MAX; K++)
//	{
//		for (int l = 0; l < PLAYER_MAX; l++)
//		{
//
//			if ((enemy->use == true) && (player->use == true))
//			{
//
//				if ((GetPlayerPos() > posGorl.x) &&
//					(player->pos.x - 20 * 0.72f < enemy->pos.x + 20))
//				{
//					if ((player->pos.y + 20 > enemy->pos.y - 20) &&
//						(player->pos.y - 20 < enemy->pos.y + 20))
//					{
//						SetEffect(enemy->pos);
//						break;
//					}
//				}
//			}
//		}
//	}
//}
