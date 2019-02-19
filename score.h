//=============================================================================
//
// �X�R�A���� [score.h]
// Author : ���c����
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
typedef struct
{

	D3DXVECTOR3				pos;						// �ʒu
	D3DXVECTOR3				rot;						// ����
} SCORE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
SCORE *GetScore(void);

void ChangeScore(int value);

#endif
