//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 
//
//=============================================================================
#ifndef _SCORE_EFFECT_H_
#define _SCORE_EFFECT_H_


// マクロ定義
#define TEXTURE_GAME_SCORE_EFFECT ("data/TEXTURE/s_recover.png")	// サンプル用画像
#define TEXTURE_SCORE_EFFECT_SIZE_X	(70)						// テクスチャサイズ
#define TEXTURE_SCORE_EFFECT_SIZE_Y	(70)						// テクスチャサイズ

#define TEXTURE_PATTERN_DIVIDE_X_SCORE_EFFECT	(5)									// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_SCORE_EFFECT	(1)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_SCORE_EFFECT			(TEXTURE_PATTERN_DIVIDE_X_SCORE_EFFECT*TEXTURE_PATTERN_DIVIDE_Y_SCORE_EFFECT)	// アニメーションパターン数
#define TIME_ANIMATION_SCORE_EFFECT			(2)										// アニメーションの切り替わるカウント

#define SCORE_EFFECT_MAX              (4)	                            //エフェクト用
#define	SCORE_RADIUS_MIN				(50.0f)							// ポリゴンの半径最小値
#define	RADIUS_MAX				(300.0f)						// ポリゴンの半径最大値
#define	SCORE_VALUE_ROTATION			(D3DX_PI * 0.01f)				// ポリゴンの回転量
#define	VALUE_SCORE_EFFECT				(2.0f)							// ポリゴンのスケール変化量
#define	NUM_SCORE_EFFECT				(2)	

//構造体宣言

typedef struct
{
	VERTEX_2D				vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	D3DXVECTOR3 			pos;						// ポリゴンの座標
	D3DXVECTOR3				rot;					  	// ポリゴンの回転量
	int						CountAnim;					// アニメーションカウント
	int						PatternAnim;				// アニメーションパターンナンバー
	bool					use;						//使用、未使用の判定
	float					Radius;						// エネミーの半径
	float					BaseAngle;					// エネミーの角度

}SCOREEFFECT;







//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScoreEffect(int type);
void UninitScoreEffect(void);
void UpdateScoreEffect(void);
void DrawScoreEffect(void);
void SetScoreEffect(D3DXVECTOR3 pos);
SCOREEFFECT *GetScoreEffect(int pno);

#endif