//=============================================================================
//
// スコア処理 [score.h]
// Author : 増田光汰
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
D3DXVECTOR3 GetScore(void);

void ChangeScore(int value);

#endif
