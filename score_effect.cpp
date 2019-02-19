//=============================================================================
//�쐬�ҁ@GP11A341_34_���c����
//�G�t�F�N�g���� [score_effect.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
//#include "enemy.h"
#include "score_effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexScoreEffect(void);
void SetTextureScoreEffect(int cntPattern);	// 
void SetVertexScoreEffect(void);			    // ���_�̌v�Z����


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DScoreEffectTexture = NULL;		// �e�N�X�`���ւ̃|�C���^


SCOREEFFECT					score_effect[SCORE_EFFECT_MAX];					//�z���

float					g_fRadiusScore;					// �|���S���̔��a
float					g_fBaseAngleScore;					// �|���S���̊p�x

//=============================================================================
// ����������
//=============================================================================
HRESULT InitScoreEffect(int type)
{
	//ENEMY *enemy = GetEnemy(0);
	//int i;

	//�G�t�F�N�g�̐^�U
	score_effect->use = false;

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_SCORE_EFFECT_SIZE_X, TEXTURE_SCORE_EFFECT_SIZE_Y);
	score_effect->Radius = D3DXVec2Length(&temp);						// �v���C���[�̔��a��������
	score_effect->BaseAngle = atan2f(TEXTURE_SCORE_EFFECT_SIZE_Y, TEXTURE_SCORE_EFFECT_SIZE_X);	// �v���C���[�̊p�x��������



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�G�t�F�N�g�̏����ʒu���G�l�~�[��pos��
	score_effect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�A�j���[�V�����J�E���g�̏�����
	score_effect->CountAnim = 0;

	//�A�j���[�V�����p�^�[���i���o�[�̏�����
	score_effect->PatternAnim = 0;

	// ���_���̍쐬
	MakeVertexScoreEffect();

	if (type = 1)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_SCORE_EFFECT,				// �t�@�C���̖��O
			&g_pD3DScoreEffectTexture);					// �ǂݍ��ރ������̃|�C���^
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScoreEffect(void)
{

	if (g_pD3DScoreEffectTexture != NULL)
	{	// �e�N�X�`���̊J��
		g_pD3DScoreEffectTexture->Release();
		g_pD3DScoreEffectTexture = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScoreEffect(void)
{
	SCOREEFFECT *score_effect = GetScoreEffect(0);
	//ENEMY *enemy = GetEnemy(0);

	for(int i=0;i<SCORE_EFFECT_MAX;i++,score_effect++)
	{

		if (score_effect->use == true)
		{
			//�A�j���[�V����
			score_effect->CountAnim++;

			// �A�j���[�V����Wait�`�F�b�N
			if ((score_effect->CountAnim % TIME_ANIMATION_SCORE_EFFECT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				score_effect->PatternAnim = (score_effect->PatternAnim + 1 % ANIM_PATTERN_NUM_SCORE_EFFECT);

				//score_effect->PatternAnim = 10;

				// �e�N�X�`�����W��ݒ�
				SetTextureScoreEffect(score_effect->PatternAnim);

				if (score_effect->PatternAnim == ANIM_PATTERN_NUM_SCORE_EFFECT)
				{
					score_effect->PatternAnim = 0;
					score_effect->use = false;
					break;
				}

				SetVertexScoreEffect();
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScoreEffect(void)
{
	SCOREEFFECT *score_effect = GetScoreEffect(0);
	LPDIRECT3DDEVICE9 Device = GetDevice();

	//���Z����
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(DEST) - �]����(SRC)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SCORE_EFFECT_MAX; i++, score_effect++)
	{
		if (score_effect->use == true)
		{

			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DScoreEffectTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SCORE_EFFECT, score_effect->vertexWk, sizeof(VERTEX_2D));

		}
	}
	// �ʏ�u�����h�ɖ߂�
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexScoreEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�	
	SetVertexScoreEffect();	// ���_�̌v�Z����

		score_effect->vertexWk[0].rhw =
		score_effect->vertexWk[1].rhw =
		score_effect->vertexWk[2].rhw =
		score_effect->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
		score_effect->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);
		score_effect->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);
		score_effect->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);
		score_effect->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTextureScoreEffect(score_effect->PatternAnim);

	return S_OK;
}


//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureScoreEffect(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_SCORE_EFFECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_SCORE_EFFECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_SCORE_EFFECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_SCORE_EFFECT;

	score_effect->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	score_effect->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	score_effect->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	score_effect->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexScoreEffect(void)	//�|���S���̍��W
{

	// ���_���W�̐ݒ�
	//score_effect->vertexWk[0].vtx = D3DXVECTOR3(score_effect->pos.x, score_effect->pos.y, score_effect->pos.z);
	//score_effect->vertexWk[1].vtx = D3DXVECTOR3(score_effect->pos.x + TEXTURE_EFFECT_SIZE_X, score_effect->pos.y, score_effect->pos.z);
	//score_effect->vertexWk[2].vtx = D3DXVECTOR3(score_effect->pos.x, score_effect->pos.y + TEXTURE_EFFECT_SIZE_Y, score_effect->pos.z);
	//score_effect->vertexWk[3].vtx = D3DXVECTOR3(score_effect->pos.x + TEXTURE_EFFECT_SIZE_X, score_effect->pos.y + TEXTURE_EFFECT_SIZE_Y, score_effect->pos.z);


	// ���_���W�̐ݒ�
	score_effect->vertexWk[0].vtx.x = score_effect->pos.x - cosf(score_effect->BaseAngle + score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[0].vtx.y = score_effect->pos.y - sinf(score_effect->BaseAngle + score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[0].vtx.z = 0.0f;

	score_effect->vertexWk[1].vtx.x = score_effect->pos.x + cosf(score_effect->BaseAngle - score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[1].vtx.y = score_effect->pos.y - sinf(score_effect->BaseAngle - score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[1].vtx.z = 0.0f;

	score_effect->vertexWk[2].vtx.x = score_effect->pos.x - cosf(score_effect->BaseAngle - score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[2].vtx.y = score_effect->pos.y + sinf(score_effect->BaseAngle - score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[2].vtx.z = 0.0f;

	score_effect->vertexWk[3].vtx.x = score_effect->pos.x + cosf(score_effect->BaseAngle + score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[3].vtx.y = score_effect->pos.y + sinf(score_effect->BaseAngle + score_effect->rot.z) * score_effect->Radius;
	score_effect->vertexWk[3].vtx.z = 0.0f;

}


//
void SetScoreEffect(D3DXVECTOR3 pos)
{
	SCOREEFFECT *score_effect = GetScoreEffect(0);

	for (int i = 0; i < SCORE_EFFECT_MAX; i++)
	{
		if ((score_effect+i)->use == false)
		{
			(score_effect+i)->pos = pos;

			(score_effect+i)->pos.y -= 10.0f;
			
			(score_effect + i)->pos.x -= 10.0f;

			(score_effect+i)->use = true;

			SetVertexScoreEffect();

			break;
		}
	}
}


//�G�t�F�N�g�����擾����
SCOREEFFECT *GetScoreEffect(int eno)
{
	return &score_effect[eno];
}
