//=============================================================================
//
// ボックスエフェクト処理 [box_effect.cpp]
// Author : 増田　光汰
//
//=============================================================================
#include "box_effect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PARTICLE		"data/MODEL/Effect_Box.x"		// 読み込むモデルファイル名
#define	TEXTURE_BOX_EFFECT	"data/TEXTURE/effect_box.png"	// 読み込むテクスチャファイル名
#define	PARTICLE_SIZE_X		(50.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)							// 移動速度

#define	MAX_PARTICLE			(512)						// ビルボード最大数

#define	DISP_SHADOW											// 影の表示

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int						PTAlpha;						// アルファテストの閾値

bool					PTAlpaTest;						// アルファテストON/OFF
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBox_Effect = NULL;	// テクスチャへのポインタ
LPD3DXMESH				D3DXMeshBox_Effect;				// メッシュ情報へのポインタ
LPD3DXBUFFER			D3DXBuffMatBox_Effect;			// マテリアル情報へのポインタ
DWORD					NumMatBox_Effect;				// マテリアル情報の数

D3DXMATRIX				g_mtxWorldBox_Effect;			// ワールドマトリックス


BOX_PARTICLE				g_aBox_Effect[MAX_PARTICLE];		// パーティクルワーク
D3DXVECTOR3				g_posBase;						// ビルボード発生位置
float					g_fWidthBase = 5.0f;			// 基準の幅
float					g_fHeightBase = 10.0f;			// 基準の高さ
bool					g_bPause = false;				// ポーズON/OFF

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBox_Effect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		TEXTURE_BOX_EFFECT,						// ファイルの名前
		&g_pD3DTextureBox_Effect);				// 読み込むメモリー
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_PARTICLE,// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatBox_Effect,					// マテリアルデータを含むバッファへのポインタ　材質情報
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatBox_Effect,						// D3DXMATERIAL構造体の数　何個のマテリルがあるか
		&D3DXMeshBox_Effect)))					// ID3DXMeshインターフェイスへのポインタのアドレス　
	{
		return E_FAIL;
	}

	for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++)
	{
		g_aBox_Effect[nCntBox_Effect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBox_Effect[nCntBox_Effect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBox_Effect[nCntBox_Effect].scale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
		g_aBox_Effect[nCntBox_Effect].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aBox_Effect[nCntBox_Effect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBox_Effect[nCntBox_Effect].fSizeX = PARTICLE_SIZE_X;
		g_aBox_Effect[nCntBox_Effect].fSizeY = PARTICLE_SIZE_Y;
		g_aBox_Effect[nCntBox_Effect].nIdxShadow = -1;
		g_aBox_Effect[nCntBox_Effect].nLife = 0;
		g_aBox_Effect[nCntBox_Effect].bUse = false;
	}

	g_posBase = D3DXVECTOR3(6000.0f, 0.0f, 0.0f);

	PTAlpha = 0;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBox_Effect(void)
{
	if (g_pD3DTextureBox_Effect != NULL)
	{// テクスチャの開放
		g_pD3DTextureBox_Effect->Release();
		g_pD3DTextureBox_Effect = NULL;
	}
	if (D3DXMeshBox_Effect != NULL)
	{// テクスチャの開放
		D3DXMeshBox_Effect->Release();
		D3DXMeshBox_Effect = NULL;
	}
	if (D3DXBuffMatBox_Effect != NULL)
	{// テクスチャの開放
		D3DXBuffMatBox_Effect->Release();
		D3DXBuffMatBox_Effect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBox_Effect(void)
{
	D3DXVECTOR3 rotCamera;
	BOX_PARTICLE *Box_Effect = g_aBox_Effect;
	// カメラの回転を取得
	rotCamera = GetRotCamera();
	// アルファテストON/OFF
	if (GetKeyboardTrigger(DIK_Y))
	{
		PTAlpaTest = PTAlpaTest ? false : true;
	}


	if (g_bPause == false)
	{
		for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++, Box_Effect++)
		{

			if (g_aBox_Effect[nCntBox_Effect].bUse)
			{// 使用中
				float t = Get(Box_Effect->pos);
				g_aBox_Effect[nCntBox_Effect].pos.x += g_aBox_Effect[nCntBox_Effect].move.x;
				g_aBox_Effect[nCntBox_Effect].pos.z += g_aBox_Effect[nCntBox_Effect].move.z;

				g_aBox_Effect[nCntBox_Effect].pos.y += g_aBox_Effect[nCntBox_Effect].move.y;
				if (g_aBox_Effect[nCntBox_Effect].pos.y < t)
				{// 着地した
					g_aBox_Effect[nCntBox_Effect].pos.y = t;
					g_aBox_Effect[nCntBox_Effect].move.y = -g_aBox_Effect[nCntBox_Effect].move.y * 0.75f;
				}

				g_aBox_Effect[nCntBox_Effect].move.x += (0.0f - g_aBox_Effect[nCntBox_Effect].move.x) * 0.015f;
				g_aBox_Effect[nCntBox_Effect].move.y -= 0.25f;
				g_aBox_Effect[nCntBox_Effect].move.z += (0.0f - g_aBox_Effect[nCntBox_Effect].move.z) * 0.015f;
				//Box_Effect->pos.y = Get(Box_Effect->pos);
#ifdef DISP_SHADOW
				if (g_aBox_Effect[nCntBox_Effect].nIdxShadow != -1)
				{// 影使用中
					float colA;

					// 影の位置設定
					SetPositionShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow, D3DXVECTOR3(g_aBox_Effect[nCntBox_Effect].pos.x, 0.1f, g_aBox_Effect[nCntBox_Effect].pos.z));

					// 影のα値設定
					if (g_aBox_Effect[nCntBox_Effect].col.a > 0.0f)
					{
						colA = (300.0f - (g_aBox_Effect[nCntBox_Effect].pos.y - 9.0f)) / (300.0f / g_aBox_Effect[nCntBox_Effect].col.a);
						if (colA < 0.0f)
						{
							colA = 0.0f;
						}
					}
					else
					{
						colA = 0.0f;
					}
				}
#endif

				g_aBox_Effect[nCntBox_Effect].nLife--;
				if (g_aBox_Effect[nCntBox_Effect].nLife <= 0)
				{
					g_aBox_Effect[nCntBox_Effect].bUse = false;
					DeleteShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow);
					g_aBox_Effect[nCntBox_Effect].nIdxShadow = -1;
				}
				else
				{
					if (g_aBox_Effect[nCntBox_Effect].nLife <= 80)
					{
						g_aBox_Effect[nCntBox_Effect].col.r = 0.60f - (float)(80 - g_aBox_Effect[nCntBox_Effect].nLife) / 8.0f * 0.06f + 0.2f;
						g_aBox_Effect[nCntBox_Effect].col.g = 0.70f - (float)(80 - g_aBox_Effect[nCntBox_Effect].nLife) / 8.0f * 0.07f;
						g_aBox_Effect[nCntBox_Effect].col.b = 0.05f;
					}

					if (g_aBox_Effect[nCntBox_Effect].nLife <= 20)
					{
						// α値設定
						g_aBox_Effect[nCntBox_Effect].col.a -= 0.05f;
						if (g_aBox_Effect[nCntBox_Effect].col.a < 0.0f)
						{
							g_aBox_Effect[nCntBox_Effect].col.a = 0.0f;
						}
					}
				}
			}
			//Box_Effect->pos.y = Get(Box_Effect->pos);
		}

		// パーティクル発生
		//if((rand() % 2) == 0)
		{
			float fAngle, fLength, tAngle;
			int nLife;
			float fSize;

			Box_Effect->pos = g_posBase;

			fAngle = (float)(rand() % 30 - 15) / 50.0f;
			fLength = rand() % (int)(g_fWidthBase * 300) / 150.0f - g_fWidthBase;
			tAngle = (float)(rand() % 30 + 15) * 50.0f;
			//動き
			Box_Effect->move.x = sinf(fAngle) * fLength * 1.4f;
			Box_Effect->move.y = rand() % 30 / 70.0f + 7.0f;
			Box_Effect->move.z = cosf(tAngle) * 1.4f;

			nLife = rand() % 150 + 10;

			fSize = (float)(rand() % 3 + 2);

			Box_Effect->pos.y = fSize / 2;

			//ボックスエフェクトの設定
			SetBox_Effect(Box_Effect->pos, Box_Effect->move, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), fSize, fSize, nLife);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBox_Effect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxView, mtxScale, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;



	for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++)
	{
		if (g_aBox_Effect[nCntBox_Effect].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBox_Effect);

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aBox_Effect[nCntBox_Effect].scale.x, g_aBox_Effect[nCntBox_Effect].scale.y, g_aBox_Effect[nCntBox_Effect].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldBox_Effect, &g_mtxWorldBox_Effect, &mtxScale);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBox_Effect[nCntBox_Effect].rot.y, g_aBox_Effect[nCntBox_Effect].rot.x, g_aBox_Effect[nCntBox_Effect].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldBox_Effect, &g_mtxWorldBox_Effect, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aBox_Effect[nCntBox_Effect].pos.x, g_aBox_Effect[nCntBox_Effect].pos.y, g_aBox_Effect[nCntBox_Effect].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBox_Effect, &g_mtxWorldBox_Effect, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBox_Effect);

			// 現在のマテリアルを取得//元の戻すためのバックアップ
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			D3DXMat = (D3DXMATERIAL*)D3DXBuffMatBox_Effect->GetBufferPointer();

			//マテリアルの数だけ回す
			for (int CntMat = 0; CntMat < (int)NumMatBox_Effect; CntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&D3DXMat[CntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0
					, g_pD3DTextureBox_Effect);

				// 描画
				D3DXMeshBox_Effect->DrawSubset(CntMat);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetBox_Effect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxBox_Effect = -1;


	for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++)
	{
		if (!g_aBox_Effect[nCntBox_Effect].bUse)
		{
			g_aBox_Effect[nCntBox_Effect].pos = pos;
			g_aBox_Effect[nCntBox_Effect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBox_Effect[nCntBox_Effect].scale = D3DXVECTOR3(0.05f, 0.05f, 0.05f);
			g_aBox_Effect[nCntBox_Effect].move.x = move.x;
			g_aBox_Effect[nCntBox_Effect].move.y= move.y;
			g_aBox_Effect[nCntBox_Effect].move.z= move.z;
			g_aBox_Effect[nCntBox_Effect].col = col;
			g_aBox_Effect[nCntBox_Effect].fSizeX = fSizeX;
			g_aBox_Effect[nCntBox_Effect].fSizeY = fSizeY;
			g_aBox_Effect[nCntBox_Effect].nLife = nLife;
			g_aBox_Effect[nCntBox_Effect].bUse = true;


			nIdxBox_Effect = nCntBox_Effect;

#ifdef DISP_SHADOW
			// 影の設定
			g_aBox_Effect[nCntBox_Effect].nIdxShadow = SetShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), 4.5, 4.5);		// 影の設定
			if (g_aBox_Effect[nCntBox_Effect].nIdxShadow != -1)
			{
				SetColorShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif
			break;
			return nIdxBox_Effect;
		}
	}

}
//=============================================================================
// ボックスエフェクトを取得
//=============================================================================
BOX_PARTICLE *GetBox_Effect(int pno)
{
	return &g_aBox_Effect[pno];
}
