//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 増田　光汰
//
//=============================================================================
#ifndef _BEZIER_PARTICLEH_
#define _BEZIER_PARTICLEH_H_

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
	int		frame;			// 到達フレーム
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
	int cntFrame;
	D3DXVECTOR3 start;		//
	D3DXVECTOR3 control1;	// ベジェ曲線用
	D3DXVECTOR3 control2;	//

} BEZIER_PARTICLEH;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBezier_Particle(void);
void UninitBezier_Particle(void);
void UpdateBezier_Particle(void);
void DrawBezier_Particle(void);
//BEZIER_PARTICLEH *GetBox_Effect(int pno);
void SetBezier_Particle(D3DXVECTOR3 pos);
BEZIER_PARTICLEH *GetBezier_Particle(void);
#endif
