//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 増田　光汰
//
//=============================================================================
#include "box_effect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PARTICLE		"data/MODEL/Effect_Box.x"	// 読み込むモデルファイル名
#define	TEXTURE_BOX_EFFECT	"data/TEXTURE/effect_box.png"	// 読み込むテクスチャファイル名
#define	PARTICLE_SIZE_X		(50.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)							// 移動速度

#define	MAX_PARTICLE			(512)						// ビルボード最大数

#define	DISP_SHADOW				// 影の表示
//#undef DISP_SHADOW

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//HRESULT MakeVertexBox_Effect(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexBox_Effect(int nIdxBox_Effect, float fSizeX, float fSizeY);
//void SetColorBox_Effect(int nIdxBox_Effect, D3DXCOLOR col);

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


PARTICLE				g_aBox_Effect[MAX_PARTICLE];		// パーティクルワーク
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

	// 頂点情報の作成
	//MakeVertexBox_Effect(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_BOX_EFFECT,			// ファイルの名前
		&g_pD3DTextureBox_Effect);	// 読み込むメモリー
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX/*初期化もしてくれる*/(MODEL_PARTICLE,		// 読み込むモデルファイル名(Xファイル)
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

	//if (g_pD3DVtxBuffBox_Effect != NULL)
	//{// 頂点バッファの開放
	//	g_pD3DVtxBuffBox_Effect->Release();
	//	g_pD3DVtxBuffBox_Effect = NULL;
	//}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBox_Effect(void)
{
	D3DXVECTOR3 rotCamera;
	PARTICLE *Box_Effect = g_aBox_Effect;
	// カメラの回転を取得
	rotCamera = GetRotCamera();
	// アルファテストON/OFF
	if (GetKeyboardTrigger(DIK_Y))
	{
		PTAlpaTest = PTAlpaTest ? false : true;
	}

	//if(GetKeyboardPress(DIK_LEFT))
	//{
	//	if(GetKeyboardPress(DIK_UP))
	//	{// 左前移動
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if(GetKeyboardPress(DIK_DOWN))
	//	{// 左後移動
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// 左移動
	//		g_posBase.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if(GetKeyboardPress(DIK_RIGHT))
	//{
	//	if(GetKeyboardPress(DIK_UP))
	//	{// 右前移動
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if(GetKeyboardPress(DIK_DOWN))
	//	{// 右後移動
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// 右移動
	//		g_posBase.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if(GetKeyboardPress(DIK_UP))
	//{// 前移動
	//	g_posBase.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	//else if(GetKeyboardPress(DIK_DOWN))
	//{// 後移動
	//	g_posBase.x -= sinf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase.z -= cosf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}

	//if (GetKeyboardPress(DIK_1))
	//{
	//	g_fWidthBase -= 0.1f;
	//	if (g_fWidthBase < 2.0f)
	//	{
	//		g_fWidthBase = 2.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_2))
	//{
	//	g_fWidthBase += 0.1f;
	//	if (g_fWidthBase > 10.0f)
	//	{
	//		g_fWidthBase = 10.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_3))
	//{
	//	g_fHeightBase -= 0.1f;
	//	if (g_fHeightBase < 5.0f)
	//	{
	//		g_fHeightBase = 5.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_4))
	//{
	//	g_fHeightBase += 0.1f;
	//	if (g_fHeightBase > 15.0f)
	//	{
	//		g_fHeightBase = 15.0f;
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	g_bPause = g_bPause ? false : true;
	//}

	if (g_bPause == false)
	{
		for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++, Box_Effect++)
		{
			if (g_aBox_Effect[nCntBox_Effect].bUse)
			{// 使用中
				g_aBox_Effect[nCntBox_Effect].pos.x += g_aBox_Effect[nCntBox_Effect].move.x;
				g_aBox_Effect[nCntBox_Effect].pos.z += g_aBox_Effect[nCntBox_Effect].move.z;

				g_aBox_Effect[nCntBox_Effect].pos.y += g_aBox_Effect[nCntBox_Effect].move.y;
				if (g_aBox_Effect[nCntBox_Effect].pos.y <= g_aBox_Effect[nCntBox_Effect].fSizeY / 2)
				{// 着地した
					g_aBox_Effect[nCntBox_Effect].pos.y = g_aBox_Effect[nCntBox_Effect].fSizeY / 2;
					g_aBox_Effect[nCntBox_Effect].move.y = -g_aBox_Effect[nCntBox_Effect].move.y * 0.75f;
				}

				g_aBox_Effect[nCntBox_Effect].move.x += (0.0f - g_aBox_Effect[nCntBox_Effect].move.x) * 0.015f;
				g_aBox_Effect[nCntBox_Effect].move.y -= 0.25f;
				g_aBox_Effect[nCntBox_Effect].move.z += (0.0f - g_aBox_Effect[nCntBox_Effect].move.z) * 0.015f;

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

					// 影の色の設定
					//SetColorShadow(g_aBox_Effect[nCntBox_Effect].nIdxShadow, D3DXCOLOR(0.15f, 0.15f, 0.15f, colA));
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

					// 色の設定
					//SetColorBox_Effect(nCntBox_Effect, g_aBox_Effect[nCntBox_Effect].col);
				}
			}
		}

		// パーティクル発生
		//if((rand() % 2) == 0)
		{
			float fAngle, fLength;
			int nLife;
			float fSize;

			Box_Effect->pos = g_posBase;

			fAngle = (float)(rand() % 30 - 15) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

			//動き
			Box_Effect->move.x = sinf(fAngle) * fLength;
			Box_Effect->move.y = rand() % 30 / 30.0f + 7.0f;
			Box_Effect->move.z = cosf(fAngle) * 0.1f;

			nLife = rand() % 100 + 10;

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
//HRESULT MakeVertexBox_Effect(LPDIRECT3DDEVICE9 pDevice)
//{
//	// オブジェクトの頂点バッファを生成
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
//		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
//		FVF_VERTEX_3D,								// 使用する頂点フォーマット
//		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
//		&g_pD3DVtxBuffBox_Effect,					// 頂点バッファインターフェースへのポインタ
//		NULL)))										// NULLに設定
//	{
//		return E_FAIL;
//	}
//
//	{//頂点バッファの中身を埋める
//		VERTEX_3D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffBox_Effect->Lock(0, 0, (void**)&pVtx, 0);
//
//		for (int nCntBox_Effect = 0; nCntBox_Effect < MAX_PARTICLE; nCntBox_Effect++, pVtx += 4)
//		{
//			// 頂点座標の設定
//			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
//			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
//			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
//			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
//
//			// 法線の設定
//			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
//
//			// 反射光の設定
//			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffBox_Effect->Unlock();
//	}
//
//	return S_OK;
//}

//=============================================================================
// 頂点座標の設定
//=============================================================================
//void SetVertexBox_Effect(int nIdxBox_Effect, float fSizeX, float fSizeY)
//{
//	{//頂点バッファの中身を埋める
//		VERTEX_3D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffBox_Effect->Lock(0, 0, (void**)&pVtx, 0);
//
//		pVtx += (nIdxBox_Effect * 4);
//
//		// 頂点座標の設定
//		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffBox_Effect->Unlock();
//	}
//}
//
//=============================================================================
// 頂点カラーの設定
//=============================================================================
//void SetColorBox_Effect(int nIdxBox_Effect, D3DXCOLOR col)
//{
//	{//頂点バッファの中身を埋める
//		VERTEX_3D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffBox_Effect->Lock(0, 0, (void**)&pVtx, 0);
//
//		pVtx += (nIdxBox_Effect * 4);
//
//		// 頂点座標の設定
//		pVtx[0].diffuse =
//			pVtx[1].diffuse =
//			pVtx[2].diffuse =
//			pVtx[3].diffuse = col;
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffBox_Effect->Unlock();
//	}
//}

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
// エフェクトを取得
//=============================================================================
PARTICLE *GetBox_Effect(int pno)
{
	return &g_aBox_Effect[pno];
}
