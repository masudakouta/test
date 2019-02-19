//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 増田光汰
//
//=============================================================================
#include "score.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/time.jpg"	// 読み込むテクスチャファイル名
#define	TEXTURE_SCORE_MARU	"data/TEXTURE/tama.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_FRAME_SCORE	"data/TEXTURE/tama.png"	// 読み込むテクスチャファイル名
#define	SCORE_SIZE_X		(35.0f)							// スコアの数字の幅
#define	SCORE_SIZE_Y		(50.0f)							// スコアの数字の高さ
#define	SCORE_INTERVAL_X	(0.0f)							// スコアの数字の表示間隔

#define	NUM_PLACE			(8)								// スコアの桁数

#define	SCORE_POS_X			(SCREEN_WIDTH - (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_PLACE - 20.0f)	// スコアの表示基準位置Ｘ座標
#define	SCORE_POS_Y			(25.0f)																	// スコアの表示基準位置Ｙ座標

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);
void SetTextureScore(int idx, int number);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureScore[3] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffScore = NULL;		// 頂点バッファインターフェースへのポインタ
	int						g_score;						// スコア
SCORE						Score;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Score.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Score.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// スコアの初期化
	g_score = 0;

	// 頂点情報の作成
	MakeVertexScore(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_SCORE,				// ファイルの名前
								&g_pD3DTextureScore[0]);	// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_FRAME_SCORE,		// ファイルの名前
								&g_pD3DTextureScore[1]);	// 読み込むメモリー

															// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_SCORE_MARU,			// ファイルの名前
								&g_pD3DTextureScore[2]);	// 読み込むメモリー


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	if(g_pD3DTextureScore[0] != NULL)
	{// テクスチャの開放
		g_pD3DTextureScore[0]->Release();
		g_pD3DTextureScore[0] = NULL;
	}
	if(g_pD3DTextureScore[1] != NULL)
	{// テクスチャの開放
		g_pD3DTextureScore[1]->Release();
		g_pD3DTextureScore[1] = NULL;
	}

	if (g_pD3DTextureScore[2] != NULL)
	{// テクスチャの開放
		g_pD3DTextureScore[2]->Release();
		g_pD3DTextureScore[2] = NULL;
	}

	if(g_pD3DVtxBuffScore != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffScore->Release();
		g_pD3DVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	for(int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		int number;
		
		number = (g_score % (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace)))) / (int)(powf(10.0f, (float)(NUM_PLACE - nCntPlace - 1)));
		SetTextureScore(nCntPlace, number);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200.0f);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pD3DVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureScore[0]);

	// ポリゴンの描画
	for(int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlace * 4), NUM_POLYGON);
	}

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureScore[1]);

	// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);

	// テクスチャの設定
	//pDevice->SetTexture(0, g_pD3DTextureScore[2]);

	// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (NUM_PLACE * 4), NUM_POLYGON);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX * NUM_PLACE + 4),	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
												FVF_VERTEX_2D,									// 使用する頂点フォーマット
												D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffScore,							// 頂点バッファインターフェースへのポインタ
												NULL)))											// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * SCORE_SIZE_X + SCORE_INTERVAL_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + nCntPlace * (SCORE_INTERVAL_X + SCORE_SIZE_X) + SCORE_SIZE_X, SCORE_POS_Y + SCORE_SIZE_Y, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}
		//フレーム
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(50.0f - 25, 0.0f - 25, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(50.0f + 190, 0.0f - 25, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(50.0f - 25, 0.0f + 195, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(50.0f + 190, 0.0f + 195, 0.0f);

			//pVtx[0].vtx = D3DXVECTOR3(90.0f, 70.0f, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(90.0f, 70.0f, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(90.0f, 70.0f, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(SCORE_POS_X + (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_PLACE + 15, SCORE_POS_Y + 55, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		//玉
		{
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffScore->Unlock();
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScore(int idx, int number)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (idx * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffScore->Unlock();
}

//=============================================================================
// スコアの変更
//=============================================================================
void ChangeScore(int value)
{
	g_score += value;
	if(g_score < 0)
	{
		g_score = 0;
	}
	else if(g_score >= (int)(powf(10.0f, (float)(NUM_PLACE + 1))))
	{
		g_score = (int)(powf(10.0f, (float)(NUM_PLACE + 1))) - 1;
	}
}

SCORE *GetScore(void)
{
	return &Score;
}
