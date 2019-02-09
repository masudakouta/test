//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : ���c�@����
//
//=============================================================================
#ifndef _BOX_EFFECT_H_
#define _BOX_EFFECT_H_

#include "main.h"
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
} PARTICLE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBox_Effect(void);
void UninitBox_Effect(void);
void UpdateBox_Effect(void);
void DrawBox_Effect(void);
PARTICLE *GetBox_Effect(int pno);
int SetBox_Effect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);

#endif
#pragma once
