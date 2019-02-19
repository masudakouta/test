//=============================================================================
//作成者　GP11A341_34_増田光汰
//エフェクト処理 [score_effect.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
//#include "enemy.h"
#include "score_effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScoreEffect(void);
void SetTextureScoreEffect(int cntPattern);	// 
void SetVertexScoreEffect(void);			    // 頂点の計算処理


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DScoreEffectTexture = NULL;		// テクスチャへのポインタ


SCOREEFFECT					score_effect[SCORE_EFFECT_MAX];					//配列で

float					g_fRadiusScore;					// ポリゴンの半径
float					g_fBaseAngleScore;					// ポリゴンの角度

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScoreEffect(int type)
{
	//ENEMY *enemy = GetEnemy(0);
	//int i;

	//エフェクトの真偽
	score_effect->use = false;

	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_SCORE_EFFECT_SIZE_X, TEXTURE_SCORE_EFFECT_SIZE_Y);
	score_effect->Radius = D3DXVec2Length(&temp);						// プレイヤーの半径を初期化
	score_effect->BaseAngle = atan2f(TEXTURE_SCORE_EFFECT_SIZE_Y, TEXTURE_SCORE_EFFECT_SIZE_X);	// プレイヤーの角度を初期化



	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//エフェクトの初期位置をエネミーのposに
	score_effect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//アニメーションカウントの初期化
	score_effect->CountAnim = 0;

	//アニメーションパターンナンバーの初期化
	score_effect->PatternAnim = 0;

	// 頂点情報の作成
	MakeVertexScoreEffect();

	if (type = 1)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_SCORE_EFFECT,				// ファイルの名前
			&g_pD3DScoreEffectTexture);					// 読み込むメモリのポインタ
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScoreEffect(void)
{

	if (g_pD3DScoreEffectTexture != NULL)
	{	// テクスチャの開放
		g_pD3DScoreEffectTexture->Release();
		g_pD3DScoreEffectTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScoreEffect(void)
{
	SCOREEFFECT *score_effect = GetScoreEffect(0);
	//ENEMY *enemy = GetEnemy(0);

	for(int i=0;i<SCORE_EFFECT_MAX;i++,score_effect++)
	{

		if (score_effect->use == true)
		{
			//アニメーション
			score_effect->CountAnim++;

			// アニメーションWaitチェック
			if ((score_effect->CountAnim % TIME_ANIMATION_SCORE_EFFECT) == 0)
			{
				// パターンの切り替え
				score_effect->PatternAnim = (score_effect->PatternAnim + 1 % ANIM_PATTERN_NUM_SCORE_EFFECT);

				//score_effect->PatternAnim = 10;

				// テクスチャ座標を設定
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
// 描画処理
//=============================================================================
void DrawScoreEffect(void)
{
	SCOREEFFECT *score_effect = GetScoreEffect(0);
	LPDIRECT3DDEVICE9 Device = GetDevice();

	//加算合成
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送先(DEST) - 転送元(SRC)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int i = 0; i < SCORE_EFFECT_MAX; i++, score_effect++)
	{
		if (score_effect->use == true)
		{

			LPDIRECT3DDEVICE9 pDevice = GetDevice();

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DScoreEffectTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SCORE_EFFECT, score_effect->vertexWk, sizeof(VERTEX_2D));

		}
	}
	// 通常ブレンドに戻す
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScoreEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定	
	SetVertexScoreEffect();	// 頂点の計算処理

		score_effect->vertexWk[0].rhw =
		score_effect->vertexWk[1].rhw =
		score_effect->vertexWk[2].rhw =
		score_effect->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
		score_effect->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);
		score_effect->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);
		score_effect->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);
		score_effect->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 0, 255, 255);

	// テクスチャ座標の設定
	SetTextureScoreEffect(score_effect->PatternAnim);

	return S_OK;
}


//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScoreEffect(int cntPattern)
{
	// テクスチャ座標の設定
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
// 頂点座標の設定
//=============================================================================
void SetVertexScoreEffect(void)	//ポリゴンの座標
{

	// 頂点座標の設定
	//score_effect->vertexWk[0].vtx = D3DXVECTOR3(score_effect->pos.x, score_effect->pos.y, score_effect->pos.z);
	//score_effect->vertexWk[1].vtx = D3DXVECTOR3(score_effect->pos.x + TEXTURE_EFFECT_SIZE_X, score_effect->pos.y, score_effect->pos.z);
	//score_effect->vertexWk[2].vtx = D3DXVECTOR3(score_effect->pos.x, score_effect->pos.y + TEXTURE_EFFECT_SIZE_Y, score_effect->pos.z);
	//score_effect->vertexWk[3].vtx = D3DXVECTOR3(score_effect->pos.x + TEXTURE_EFFECT_SIZE_X, score_effect->pos.y + TEXTURE_EFFECT_SIZE_Y, score_effect->pos.z);


	// 頂点座標の設定
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


//エフェクト情報を取得する
SCOREEFFECT *GetScoreEffect(int eno)
{
	return &score_effect[eno];
}
