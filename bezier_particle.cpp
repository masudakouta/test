//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 増田　光汰
//
//=============================================================================
#include "bezier_particle.h"
#include "camera.h"
#include "score.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/explosionFlare.png"	// 読み込むテクスチャファイル名
#define	PARTICLE_SIZE_X		(5.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(5.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(2.0f)							// 移動速度

#define	MAX_PARTICLE			(512)						// ビルボード最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 Device);
void SetVertexParticle(int nIdxBullet, float fSizeX, float fSizeY);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureParticle = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffParticle = NULL;			// 頂点バッファインターフェースへのポインタ
D3DXMATRIX				mtxWorldParticle;					// ワールドマトリックス

BEZIER_PARTICLEH		Particle[MAX_PARTICLE];				// パーティクルワーク
static int				PTAlpha;							// アルファテストの閾値

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点情報の作成
	MakeVertexParticle(Device);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,		// デバイスへのポインタ
		TEXTURE_PARTICLE,					// ファイルの名前
		&D3DTextureParticle);				// 読み込むメモリー

	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		Particle[CntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Particle[CntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Particle[CntParticle].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Particle[CntParticle].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Particle[CntParticle].fSizeX = PARTICLE_SIZE_X;
		Particle[CntParticle].fSizeY = PARTICLE_SIZE_Y;
		Particle[CntParticle].bUse = false;
	}

	return S_OK;

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	if (D3DTextureParticle != NULL)
	{// テクスチャの開放
		D3DTextureParticle->Release();
		D3DTextureParticle = NULL;
	}

	if (D3DVtxBuffParticle != NULL)
	{// 頂点バッファの開放
		D3DVtxBuffParticle->Release();
		D3DVtxBuffParticle = NULL;
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	D3DXVECTOR3 GetPositionPlayer();
	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		if (Particle[CntParticle].bUse)
		{
			Particle[CntParticle].cntFrame++;
			float t = (float)Particle[CntParticle].cntFrame / 60.0f;
			//n乗を求める関数 powf(1-t, n))
			D3DXVECTOR3 p0 = (1 - t) * (1 - t) * (1 - t) * Particle[CntParticle].start;
			D3DXVECTOR3 p1 = 3 * (1 - t) * (1 - t) * t * Particle[CntParticle].control1;
			D3DXVECTOR3 p2 = 3 * (1 - t) * t * t * Particle[CntParticle].control2;
			D3DXVECTOR3 p3 = t * t * t *GetPositionPlayer()/*D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/;//
			Particle[CntParticle].pos = p0 + p1 + p2 + p3;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	//フォグが悪さしないように
	Device->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// ライティングを無効に
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// パーティクルっぽく設定をする
	//Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//加算合成
	Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送先(DEST) - 転送元(SRC)
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Z比較なし
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);


	// αテストを有効に
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, PTAlpha);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{
		if (Particle[CntParticle].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldParticle);

			// ビューマトリックスを取得
			//mtxView = GetMtxView();

			//mtxWorldParticle._11 = mtxView._11;
			//mtxWorldParticle._12 = mtxView._21;
			//mtxWorldParticle._13 = mtxView._31;
			//mtxWorldParticle._21 = mtxView._12;
			//mtxWorldParticle._22 = mtxView._22;
			//mtxWorldParticle._23 = mtxView._32;
			//mtxWorldParticle._31 = mtxView._13;
			//mtxWorldParticle._32 = mtxView._23;
			//mtxWorldParticle._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, Particle[CntParticle].scale.x, Particle[CntParticle].scale.y, Particle[CntParticle].scale.z);
			D3DXMatrixMultiply(&mtxWorldParticle, &mtxWorldParticle, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, Particle[CntParticle].pos.x, Particle[CntParticle].pos.y, Particle[CntParticle].pos.z);
			D3DXMatrixMultiply(&mtxWorldParticle, &mtxWorldParticle, &mtxTranslate);

			// ワールドマトリックスの設定
			Device->SetTransform(D3DTS_WORLD, &mtxWorldParticle);

			// 頂点バッファをデバイスのデータストリームにバインド
			Device->SetStreamSource(0, D3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			Device->SetTexture(0, D3DTextureParticle);

			// ポリゴンの描画
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntParticle * 4), NUM_POLYGON);
		}
	}

	// ライティングを有効に
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//Z比較を戻す
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	Device->SetRenderState(D3DRS_FOGENABLE, TRUE);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 Device)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffParticle,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		// 頂点データをアンロックする
		D3DVtxBuffParticle->Unlock();
	}

	return S_OK;

}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffParticle->Unlock();
	}

}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		D3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetParticle(D3DXVECTOR3 pos)
{
	//未使用を目指す
	for (int CntParticle = 0; CntParticle < MAX_PARTICLE; CntParticle++)
	{

		if (!Particle[CntParticle].bUse)
		{

			Particle[CntParticle].start = pos;
			Particle[CntParticle].control1 = D3DXVECTOR3(-10.0f, 50.0f, 0.0f) + pos;
			Particle[CntParticle].control2 = D3DXVECTOR3(10.0f, 50.0f, 0.0f) + pos;
			Particle[CntParticle].cntFrame = 0;
			Particle[CntParticle].bUse = true;
			return;

		}
	}
}

BEZIER_PARTICLEH *GetParticle(void)
{
	return Particle;
}
