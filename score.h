//=============================================================================
//
// �X�R�A���� [score.h]
// Author : ���c����
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
D3DXVECTOR3 GetScore(void);

void ChangeScore(int value);

#endif