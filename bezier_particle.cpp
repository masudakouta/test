//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : ���c�@����
//
//=============================================================================
#include "bezier_particle.h"
#include "camera.h"
#include "score.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/explosionFlare.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_SIZE_X		(5.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(5.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(2.0f)							// �ړ����x

#define	MAX_PARTICLE			(512)						// �r���{�[�h�ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 Device);
void SetVertexParticle(int nIdxBullet, float fSizeX, float fSizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureParticle = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffParticle = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				mtxWorldParticle;					// ���[���h�}�g���b�N�X

BEZIER_PARTICLEH		Particle[MAX_PARTICLE];				// �p�[�e�B�N�����[�N
static int				PTAlpha;							// �A���t�@�e�X�g��臒l

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexParticle(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PARTICLE,					// �t�@�C���̖��O
		&D3DTextureParticle);				// �ǂݍ��ރ������[

	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		Particle[CntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Particle[CntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Particle[CntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Particle[CntParticle].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Particle[CntParticle].fSizeX = PARTICLE_SIZE_X;
		Particle[CntParticle].fSizeY = PARTICLE_SIZE_Y;
		Particle[CntParticle].bUse = false;
	}

	return S_OK;

}
//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	if (D3DTextureParticle != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureParticle->Release();
		D3DTextureParticle = NULL;
	}

	if (D3DVtxBuffParticle != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffParticle->Release();
		D3DVtxBuffParticle = NULL;
	}

}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	D3DXVECTOR3 GetPositionPlayer();
	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		if (Particle[CntParticle].bUse)
		{
			Particle[CntParticle].cntFrame++;
			float t = (float)Particle[CntParticle].cntFrame / 60.0f;
			//n������߂�֐� powf(1-t, n))
			D3DXVECTOR3 p0 = (1 - t) * (1 - t) * (1 - t) * Particle[CntParticle].start;
			D3DXVECTOR3 p1 = 3 * (1 - t) * (1 - t) * t * Particle[CntParticle].control1;
			D3DXVECTOR3 p2 = 3 * (1 - t) * t * t * Particle[CntParticle].control2;
			D3DXVECTOR3 p3 = t * t * t *GetPositionPlayer()/*D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/;//
			Particle[CntParticle].pos = p0 + p1 + p2 + p3;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//�t�H�O���������Ȃ��悤��
	Device->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// ���C�e�B���O�𖳌���
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �p�[�e�B�N�����ۂ��ݒ������
	//Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//���Z����
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(DEST) - �]����(SRC)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z��r�Ȃ�
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);


	// ���e�X�g��L����
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, PTAlpha);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		if (Particle[CntParticle].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldParticle);

			// �r���[�}�g���b�N�X���擾
			//mtxView = GetMtxView();

			//mtxWorldParticle._11 = mtxView._11;
			//mtxWorldParticle._12 = mtxView._21;
			//mtxWorldParticle._13 = mtxView._31;
			//mtxWorldParticle._21 = mtxView._12;
			//mtxWorldParticle._22 = mtxView._22;
			//mtxWorldParticle._23 = mtxView._32;
			//mtxWorldParticle._31 = mtxView._13;
			//mtxWorldParticle._32 = mtxView._23;
			//mtxWorldParticle._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, Particle[CntParticle].scale.x, Particle[CntParticle].scale.y, Particle[CntParticle].scale.z);
			D3DXMatrixMultiply(&mtxWorldParticle, &mtxWorldParticle, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, Particle[CntParticle].pos.x, Particle[CntParticle].pos.y, Particle[CntParticle].pos.z);
			D3DXMatrixMultiply(&mtxWorldParticle, &mtxWorldParticle, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &mtxWorldParticle);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureParticle);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntParticle * 4), NUM_POLYGON);
		}
	}

	// ���C�e�B���O��L����
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//Z��r��߂�
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	Device->SetRenderState(D3DRS_FOGENABLE, TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffParticle,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffParticle->Unlock();
	}

	return S_OK;

}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffParticle->Unlock();
	}

}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void SetParticle(D3DXVECTOR3 pos)
{
	//���g�p��ڎw��
	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{

		if (!Particle[CntParticle].bUse)
		{

			Particle[CntParticle].start = pos;
			Particle[CntParticle].control1 = D3DXVECTOR3(-10.0f, 50.0f, 0.0f) + pos;
			Particle[CntParticle].control2 = D3DXVECTOR3(10.0f, 50.0f, 0.0f) + pos;
			Particle[CntParticle].cntFrame = 0;
			Particle[CntParticle].bUse = true;
			return;

		}
	}
}

BEZIER_PARTICLEH *GetParticle(void)
{
	return Particle;
}
