//=============================================================================
//
// �x�W�F�Q�p�[�e�B�N������ [bezier_particle.cpp]
// Author : ���c�@����
//
//=============================================================================
#include "bezier_particle.h"
#include "camera.h"
#include "score.h"
#include "player.h"
#include "particle.h"
#include "score_effect.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/explosionFlare.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PARTICLE_SIZE_X		(5.0f)								// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(5.0f)								// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(2.0f)								// �ړ����x

#define	MAX_PARTICLE			(512)							// �r���{�[�h�ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexBezier_Particle(LPDIRECT3DDEVICE9 Device);
void SetVertexBezier_Particle(int nIdxBullet, float fSizeX, float fSizeY);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureParticle = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffParticle = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				mtxWorldParticle;					// ���[���h�}�g���b�N�X

BEZIER_PARTICLEH		Bezier_Particle[MAX_PARTICLE];				// �p�[�e�B�N�����[�N
static int				PTAlpha;							// �A���t�@�e�X�g��臒l

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBezier_Particle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexBezier_Particle(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PARTICLE,					// �t�@�C���̖��O
		&D3DTextureParticle);				// �ǂݍ��ރ������[

	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		Bezier_Particle[CntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bezier_Particle[CntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Bezier_Particle[CntParticle].scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
		Bezier_Particle[CntParticle].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Bezier_Particle[CntParticle].fSizeX = PARTICLE_SIZE_X;
		Bezier_Particle[CntParticle].fSizeY = PARTICLE_SIZE_Y;
		Bezier_Particle[CntParticle].bUse = false;
	}

	return S_OK;

}
//=============================================================================
// �I������
//=============================================================================
void UninitBezier_Particle(void)
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
void UpdateBezier_Particle(void)
{
	D3DXVECTOR3 GetPositionPlayer();
	PARTICLE *Particle = GetParticlet(0);

	/*��������X�N���[�����W�ϊ��̃v���O����*/
	/*�J�[�\���̈ʒu�����[���h���W�֕ϊ�*/

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�s��A�v���W�F�N�V�����s��A�r���[�|�[�g�s��
	D3DXMATRIX mtxView, mtxProjection, mtxViewPort;

	//�r���[�|�[�g�s����쐬
	D3DXMatrixIdentity(&mtxViewPort);										//������
	mtxViewPort._11 = SCREEN_WIDTH / 2.0f;									//�e�������X�N���[���T�C�Y����v�Z
	mtxViewPort._22 = -SCREEN_HEIGHT / 2.0f;
	mtxViewPort._41 = SCREEN_WIDTH / 2.0f;
	mtxViewPort._42 = SCREEN_HEIGHT / 2.0f;

	//�v���W�F�N�V�����s��擾
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

	//�r���[�s��擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�r���[�t�s��A�v���W�F�N�V�����t�s��A�r���[�|�[�g�s��
	D3DXMATRIX invView, invProjection, invViewport;

	//�t�s����쐬
	D3DXMatrixInverse(&invView, NULL, &mtxView);
	D3DXMatrixInverse(&invProjection, NULL, &mtxProjection);
	D3DXMatrixInverse(&invViewport, NULL, &mtxViewPort);

	//�J�[�\�����W���擾(���ۂ͂����̕������X�R�A�̃X�N���[�����W�ɒu��������)
	SCORE *Score;
	Score = GetScore();												//�J�[�\���̃X�N���[�����W���擾
	D3DXVECTOR3 screenPos = D3DXVECTOR3(937, 70, 0.0f);		//D3DXVECTOR3�^�ɕϊ�

	//�J�[�\���̃X�N���[�����W�����[���h���W�ɕϊ�
	D3DXVECTOR3 worldPos;												//�ϊ���̃��[���h���W���i�[����ϐ�
	D3DXMATRIX temp = invViewport * invProjection * invView;			//�ϊ�����s����쐬
	D3DXVec3TransformCoord(&worldPos, &screenPos, &temp);				//�X�N���[�����W��ϊ��s��Ń��[���h���W�֕ϊ�


	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++, Particle++)
	{
		if (Bezier_Particle[CntParticle].bUse)
		{
			Bezier_Particle[CntParticle].cntFrame++;
			float t = (float)Bezier_Particle[CntParticle].cntFrame / Bezier_Particle[CntParticle].frame;
			//n������߂�֐� powf(1-t, n))
			D3DXVECTOR3 p0 = (1 - t) * (1 - t) * (1 - t) * Bezier_Particle[CntParticle].start;
			D3DXVECTOR3 p1 = 3 * (1 - t) * (1 - t) * t * Bezier_Particle[CntParticle].control1;
			D3DXVECTOR3 p2 = 3 * (1 - t) * t * t * Bezier_Particle[CntParticle].control2;
			D3DXVECTOR3 p3 = t * t * t * worldPos;
			Bezier_Particle[CntParticle].pos = p0 + p1 + p2 + p3;

			//for (int i = 0; i < 5; i++, Particle++)
			//{
				if (Bezier_Particle[CntParticle].pos == worldPos)
				{
					//SetParticle(worldPos, Particle->move, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f, 10);
					Bezier_Particle[CntParticle].bUse = false;
					SetScoreEffect(D3DXVECTOR3(937.0f,70.0f,0.0f));
					ChangeScore(1);

				}
			//}
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBezier_Particle(void)
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
		if (Bezier_Particle[CntParticle].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldParticle);

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, Bezier_Particle[CntParticle].scale.x, Bezier_Particle[CntParticle].scale.y, Bezier_Particle[CntParticle].scale.z);
			D3DXMatrixMultiply(&mtxWorldParticle, &mtxWorldParticle, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, Bezier_Particle[CntParticle].pos.x, Bezier_Particle[CntParticle].pos.y, Bezier_Particle[CntParticle].pos.z);
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

	// �t�H�O�̐ݒ�
	Device->SetRenderState(D3DRS_FOGENABLE, TRUE);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBezier_Particle(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuffParticle,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
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
void SetVertexBezier_Particle(int nIdxParticle, float fSizeX, float fSizeY)
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
void SetColorBezier_Particle(int nIdxParticle, D3DXCOLOR col)
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
void SetBezier_Particle(D3DXVECTOR3 pos)
{
	//���g�p��ڎw��
	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{

		if (!Bezier_Particle[CntParticle].bUse)
		{

			Bezier_Particle[CntParticle].start = pos;
			Bezier_Particle[CntParticle].control1 = D3DXVECTOR3((float)(rand() % 1200) / 10, 50.0f, (float)(rand() % 1200) / 10) + pos;
			Bezier_Particle[CntParticle].control2 = D3DXVECTOR3(10.0f, 50.0f, 0.0f) + pos;
			Bezier_Particle[CntParticle].cntFrame = 0;
			Bezier_Particle[CntParticle].bUse = true;
			Bezier_Particle[CntParticle].frame = (rand() % 50) + 80;
			return;

		}
	}
}
//=============================================================================
// �֐��擾
//=============================================================================
BEZIER_PARTICLEH *GetBezier_Particle(void)
{
	return Bezier_Particle;
}
