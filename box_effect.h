//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 増田　光汰
//
//=============================================================================
#ifndef _BOX_EFFECT_H_
#define _BOX_EFFECT_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
} PARTICLE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBox_Effect(void);
void UninitBox_Effect(void);
void UpdateBox_Effect(void);
void DrawBox_Effect(void);
PARTICLE *GetBox_Effect(int pno);
int SetBox_Effect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);

#endif
#pragma once
