//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : ���c����
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "item.h"
#include "score.h"
#include "meshfield.h"
#include "fade.h"
#include "box_effect.h"
#include "particle.h"
#include "bezier_particle.h"
#include "score_effect.h"
#include "explosion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//���f��
#define	MODEL_PLAYER		"data/PLAYER/nitousinn.x"	// �ǂݍ��ރ��f����
#define TEXTURE_FILENAME	"data/PLAYER/kao.png"		//�e�N�X�`��
#define	PLAYER_RADIUS		(10.0f)						// ���a
#define	VALUE_MOVE_PLAYER	(0.155f)					// �ړ����x
#define	RATE_MOVE_PLAYER	(0.125f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.10f)						// ��]�����W��
#define	VALUE_MOVE_BULLET	(7.5f)						// �e�̈ړ����x

//��
#define PLAYER_WEAPON		"data/PLAYER/weapon.x"		// �ǂݍ��ރ��f����
#define TEXTURE_WEAPON		"data/PLAYER/weapon.png"	// �ǂݍ��ރ��f����
#define WEAPON_RADIUS		(20.0f)						// ���a
#define	VALUE_ROTATE_WEAPON	(D3DX_PI * 5.025f)			// ��]���x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void DrawPlayer_Sword(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//���f��
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer;		// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			g_pMeshPlayer;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXMatBuffPlayer;		// ���b�V���̃}�e���A�������i�[
DWORD				g_nNumMatPlayer;			// �������̑���
//��
LPDIRECT3DTEXTURE9	D3DTextureWeapon;		// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			MeshWeapon;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXMatBuffWeapon;		// ���b�V���̃}�e���A�������i�[
DWORD				NumMatWeapon;			// �������̑���
float				fLengthIntervalWeapon;
D3DXMATRIX			g_mtxWorldPlayer;			// ���[���h�}�g���b�N�X
D3DXMATRIX			mtxWorldWeapon;				// �E���[���h�}�g���b�N�X

PLAYER				g_player;					// �v���C���[���[�N
WEAPON				weapon;						// �E�F�|�����[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�P�[���̂Ȃ�₩���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//���f���̏�����
	g_pD3DTexturePlayer = NULL;
	g_pMeshPlayer = NULL;
	g_pD3DXMatBuffPlayer = NULL;
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.fRadius = PLAYER_RADIUS;

	//����̏�����
	D3DTextureWeapon = NULL;
	MeshWeapon = NULL;
	D3DXMatBuffWeapon = NULL;
	weapon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	weapon.rot = D3DXVECTOR3(3.4f, 0.0f, 0.0f);
	weapon.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	weapon.fRadius = WEAPON_RADIUS;



	// X�t�@�C���̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(MODEL_PLAYER,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_pD3DXMatBuffPlayer,
								NULL,
								&g_nNumMatPlayer,
								&g_pMeshPlayer)))
	{
		return E_FAIL;
	}
	// ����X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX/*�����������Ă����*/(PLAYER_WEAPON,// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
								D3DXMESH_SYSTEMMEM,					// ���b�V���̍쐬�I�v�V�������w��
								pDevice,							// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
								NULL,								// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
								&D3DXMatBuffWeapon,					// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^�@�ގ����
								NULL,								// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
								&NumMatWeapon,						// D3DXMATERIAL�\���̂̐��@���̃}�e���������邩
								&MeshWeapon)))						// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�@
	{
		return E_FAIL;
	}

#if 1
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,		// �t�@�C���̖��O
								&g_pD3DTexturePlayer);	// �ǂݍ��ރ������[
														// �e�N�X�`���̓ǂݍ���
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_WEAPON,			// �t�@�C���̖��O
								&D3DTextureWeapon);		// �ǂݍ��ރ������[

#endif

	// �e�̐ݒ�
	g_player.nIdxShadow = SetShadow(g_player.pos,
	g_player.fRadius * 2.0f, g_player.fRadius * 2.0f);	// �e�̐ݒ�

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	//////////
	//���f��//
	//////////
	if(g_pD3DTexturePlayer != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}

	if(g_pMeshPlayer != NULL)
	{// ���b�V���̊J��
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	if(g_pD3DXMatBuffPlayer != NULL)
	{// �}�e���A���̊J��
		g_pD3DXMatBuffPlayer->Release();
		g_pD3DXMatBuffPlayer = NULL;
	}
	//////
	//��//
	//////
	if (D3DTextureWeapon != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureWeapon->Release();
		D3DTextureWeapon = NULL;
	}

	if (MeshWeapon != NULL)
	{// ���b�V���̊J��
		MeshWeapon->Release();
		MeshWeapon = NULL;
	}

	if (D3DXMatBuffWeapon != NULL)
	{// �}�e���A���̊J��
		D3DXMatBuffWeapon->Release();
		D3DXMatBuffWeapon = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 rotCamera;
	float fDiffRotY;

	// �J�����̌����擾
	rotCamera = GetRotCamera();

	if(GetKeyboardPress(DIK_A))
	{
		if(GetKeyboardPress(DIK_W))
		{// ���O�ړ�
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_S))
		{// ����ړ�
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.25f;
		}
		else
		{// ���ړ�
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_D))
	{
		if(GetKeyboardPress(DIK_W))
		{// �E�O�ړ�
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;


			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_S))
		{// �E��ړ�
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.25f;
		}
		else
		{// �E�ړ�
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_W))
	{// �O�ړ�
		g_player.move.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = D3DX_PI + rotCamera.y;

	}
	else if(GetKeyboardPress(DIK_S))
	{// ��ړ�
		g_player.move.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = rotCamera.y;
	}


	// �e����
	if(GetKeyboardTrigger(DIK_RETURN))
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 move;
		//SetFade(FADE_OUT);

		pos.x = g_player.pos.x - sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y;
		pos.z = g_player.pos.z - cosf(g_player.rot.y) * 10.0f;

		move.x = -sinf(g_player.rot.y) * VALUE_MOVE_BULLET;
		move.y = 0.0f;
		move.z = -cosf(g_player.rot.y) * VALUE_MOVE_BULLET;

		SetBullet(pos, move, 4.0f, 4.0f, 60 * 4);

		// SE�Đ�
		//PlaySound(SOUND_LABEL_SE_SHOT);
	}

	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = g_player.rotDest.y - g_player.rot.y;
	if(fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if(fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if(g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}
	if(g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}


	weapon.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (weapon.rot.y > D3DX_PI)
	{
		weapon.rot.y -= D3DX_PI * 2.0f;
	}
	if (weapon.rot.y < -D3DX_PI)
	{
		weapon.rot.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	if(g_player.pos.x < -630.0f)
	{
		g_player.pos.x = -630.0f;
	}
	if(g_player.pos.x > 630.0f)
	{
		g_player.pos.x = 630.0f;
	}
	if(g_player.pos.y < 10.0f)
	{
		g_player.pos.y = 10.0f;
	}
	if(g_player.pos.y > 150.0f)
	{
		g_player.pos.y = 150.0f;
	}
	if(g_player.pos.z > 630.0f)
	{
		g_player.pos.z = 630.0f;
	}
	if(g_player.pos.z < -630.0f)
	{
		g_player.pos.z = -630.0f;
	}

	// �ړ��ʂɊ�����������
	g_player.move.x += (0.0f - g_player.move.x) * RATE_MOVE_PLAYER;
	g_player.move.y += (0.0f - g_player.move.y) * RATE_MOVE_PLAYER;
	g_player.move.z += (0.0f - g_player.move.z) * RATE_MOVE_PLAYER;

	weapon.move.x += (0.0f - weapon.move.x) * RATE_MOVE_PLAYER;
	weapon.move.y += (0.0f - weapon.move.y) * RATE_MOVE_PLAYER;
	weapon.move.z += (0.0f - weapon.move.z) * RATE_MOVE_PLAYER;


	if((g_player.move.x * g_player.move.x
	+ g_player.move.y * g_player.move.y
	+ g_player.move.z * g_player.move.z) > 1.0f)
	{
		D3DXVECTOR3 pos;

		pos.x = g_player.pos.x + sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y + 2.0f;
		pos.z = g_player.pos.z + cosf(g_player.rot.y) * 10.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.50f), 14.0f, 14.0f, 20);
		//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//					D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
		//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//					D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
	}

	// �G�l�~�[�Ƃ̓����蔻��
	{
		ITEM *pItem;
		BOX_PARTICLE *Box_Effect = GetBox_Effect(0);
		PARTICLE *Particle = GetParticlet(0);

		float fLength;

		pItem = GetItem();
		if (GetKeyboardTrigger(DIK_P))
		{
				weapon.rot.x -= 15.0f;
			for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
			{
				if (pItem->bUse == true)
				{
					float fLength;

					fLength = (weapon.worldPos.x - pItem->pos.x) * (weapon.worldPos.x - pItem->pos.x)
						+ (weapon.worldPos.y - pItem->pos.y) * (weapon.worldPos.y - pItem->pos.y) * 2
						+ (weapon.worldPos.z - pItem->pos.z) * (weapon.worldPos.z - pItem->pos.z);
					if (fLength < (weapon.fRadius + pItem->fRadius) * (weapon.fRadius + pItem->fRadius))
					{
						for (int i = 0; i < 30; i++, Box_Effect++,Particle++)
						{
							SetBox_Effect(pItem->pos, Box_Effect->move, D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f), 20.0f, 20.0f, 100);
							SetParticle(pItem->pos, Particle->move, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 20.0f, 20.0f, 50);
							SetBezier_Particle(pItem->pos);
							SetExplosion(pItem->pos, 10.0f, 10.0f);
						}

						// �X�R�A���Z
						pItem->bUse = false;
						// SE�Đ�
						//PlaySound(SOUND_LABEL_SE_COIN);
					}
				}
			}
		}
		if (GetKeyboardRelease(DIK_P))
		{
			weapon.rot.x += 15.0f;

		}

	}
	g_player.pos.y = Get(g_player.pos);
	weapon.pos.y = Get(weapon.pos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffPlayer->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatPlayer; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		g_pMeshPlayer->DrawSubset(nCntMat);
	}

	{// �}�e���A�����f�t�H���g�ɖ߂�
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
		//�q�̏���
		DrawPlayer_Sword();

	}
}
//=============================================================================
//�� �`�揈��
//=============================================================================


void DrawPlayer_Sword(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorldWeapon);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, weapon.rot.x, 0.0, weapon.rot.z);
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 2.6f, -2.0f);
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, &mtxTranslate);


	//����(�e)�̃��[���h�}�g���N�X�̃��[���h�}�g���N�X��������
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, GetPlayerWorldMatrix());
	D3DXVec3TransformCoord(&weapon.worldPos, &weapon.pos, &mtxWorldWeapon);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldWeapon);

	// ���݂̃}�e���A�����擾//���̖߂����߂̃o�b�N�A�b�v
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffWeapon->GetBufferPointer();

	//�}�e���A���̐�������
	for (int nCntMat = 0; nCntMat < (int)NumMatWeapon; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0
			, D3DTextureWeapon);

		// �`��
		MeshWeapon->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �v���C���[���擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_player.pos;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_player.rot;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_player.rotDest;
}

//=============================================================================
// �ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_player.move;
}
//=============================================================================
// ���f��(�e)�̃��[���h�}�g���b�N�X�擾
//=============================================================================
D3DXMATRIX *GetPlayerWorldMatrix(void)
{
	return &g_mtxWorldPlayer;
}
