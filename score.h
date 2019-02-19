//=============================================================================
//
// スコア処理 [score.h]
// Author : 増田光汰
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
typedef struct
{

	D3DXVECTOR3				pos;						// 位置
	D3DXVECTOR3				rot;						// 向き
} SCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
SCORE *GetScore(void);

void ChangeScore(int value);

#endif
