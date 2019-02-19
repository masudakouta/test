//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
// Author : 増田光汰
//
//=============================================================================
#include "meshfield.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/field000.jpg"		// 読み込むテクスチャファイル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 D3DTextureField;			// テクスチャ読み込み場所
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DINDEXBUFFER9 D3DIdxBuffField;		// インデックスバッファインターフェースへのポインタ

D3DXMATRIX mtxWorldField;						// ワールドマトリックス
D3DXVECTOR3 posField;							// ポリゴン表示位置の中心座標
D3DXVECTOR3 rotField;							// ポリゴンの回転角

int NumBlockXField, NumBlockZField;		// ブロック数
int NumVertexField;							// 総頂点数	
int NumVertexIndexField;						// 総インデックス数
int NumPolygonField;							// 総ポリゴン数
float BlockSizeXField, BlockSizeZField;	// ブロックサイズ
float Xrand, Zrand, Rrand;
float valField;
float xMax, zMax, rMax;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	xMax = (fBlockSizeX * nNumBlockX) / 2;
	zMax = (fBlockSizeX * nNumBlockX) / 2;
	rMax = 100;

	valField = (fBlockSizeX * nNumBlockX);

	// ポリゴン表示位置の中心座標を設定
	posField = pos;

	rotField = rot;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&D3DTextureField);	// 読み込むメモリー

	// ブロック数の設定
	NumBlockXField = nNumBlockX;
	NumBlockZField = nNumBlockZ;

	// 頂点数の設定
	NumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	NumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	NumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	BlockSizeXField = fBlockSizeX;
	BlockSizeZField = fBlockSizeZ;

	// オブジェクトの頂点バッファを生成
    if(FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&D3DVtxBuffField,				// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED( pDevice->CreateIndexBuffer(sizeof(WORD) * NumVertexIndexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&D3DIdxBuffField,					// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < (NumBlockZField + 1); nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (NumBlockXField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].vtx.x = -(NumBlockXField / 2.0f) * BlockSizeXField + nCntVtxX * BlockSizeXField;
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].vtx.z = (NumBlockZField / 2.0f) * BlockSizeZField - nCntVtxZ * BlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// 反射光の設定
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// 頂点データをアンロックする
		D3DVtxBuffField->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		D3DIdxBuffField->Lock( 0, 0, (void**)&pIdx, 0 );

		int nCntIdx = 0;
		for(int nCntVtxZ = 0; nCntVtxZ < NumBlockZField; nCntVtxZ++)
		{
			if(nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (NumBlockXField + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (NumBlockXField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (NumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (NumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxZ < (NumBlockZField - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (NumBlockXField + 1) + NumBlockXField;
				nCntIdx++;
			}
		}
#if 1
		// フラクタル理論　断層フラクタル　ランダム地形生成
		for (int nCntVtxX = 0; nCntVtxX < (NumBlockXField + 1); nCntVtxX++)
		{
			Xrand = (float)(rand() % (int)(valField)) - xMax;
			Zrand = (float)(rand() % (int)(valField)) - zMax;
			Rrand = (float)(rand() % 200 * (D3DX_PI)) / 100.0f;

			D3DXVECTOR3 vecRand, vecMesh, cross;
			D3DXVECTOR3 tmp;
			VERTEX_3D	*pVtx;
			D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

			vecRand.x = cosf(Rrand);
			vecRand.z = sinf(Rrand);
			vecRand.y = 0.0f;

			for (int z = 0; z < (NumBlockZField + 1); z++)
			{
				for (int x = 0; x < (NumBlockXField + 1); x++)
				{
					tmp.x = pVtx[z * (NumBlockXField + 1) + x].vtx.x;
					tmp.z = pVtx[z * (NumBlockXField + 1) + x].vtx.z;

					vecMesh.x = tmp.x - Xrand;
					vecMesh.z = tmp.z - Zrand;
					vecMesh.y = 0.0f;

					D3DXVec3Cross(&cross, &vecRand, &vecMesh);

					if (cross.y > 0.0f)
					{
						pVtx[z * (NumBlockXField + 1) + x].vtx.y += (float)(rand() % 1 + 1);
					}
					else
					{
						pVtx[z * (NumBlockXField + 1) + x].vtx.y -= (float)(rand() % 1 + 1);
					}
				}

			}
			D3DIdxBuffField->Unlock();
		}
#endif
		// インデックスデータをアンロックする
		D3DIdxBuffField->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	if(D3DVtxBuffField)
	{// 頂点バッファの開放
		D3DVtxBuffField->Release();
		D3DVtxBuffField = NULL;
	}

	if(D3DIdxBuffField)
	{// インデックスバッファの開放
		D3DIdxBuffField->Release();
		D3DIdxBuffField = NULL;
	}

	if(D3DTextureField)
	{// テクスチャの開放
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

	// フラクタル理論　断層フラクタル　ランダム地形生成
	if (GetKeyboardPress(DIK_X))
	{

	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorldField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotField.y, rotField.x, rotField.z);
	D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, posField.x, posField.y, posField.z);
	D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldField);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(D3DIdxBuffField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, D3DTextureField);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertexField, 0, NumPolygonField);
}
//=============================================================================
// 取得関数
//=============================================================================

float Get(D3DXVECTOR3 no)
{
	//頂点バッファの中身を埋める
	VERTEX_3D *Vtx;
	float posy;
	D3DXVECTOR3 cross, vec1, vec2;
	float fx = (NumBlockXField / 2.0f + (no.x / BlockSizeXField));//Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.x = - * BlockSizeXField + CntVtxX * BlockSizeXField;
	float fz = (NumBlockZField / 2.0f - (no.z / BlockSizeZField));

	int x = (int)floorf(fx);
	int z = (int)floorf(fz);

	//PrintDebugProc("fx : %f fz : %f x : %d z : %d", fx, fz, x, z);

	int vtx0, vtx1, vtx2, vtx3;

	vtx0 = z * (NumBlockXField + 1) + x;
	vtx1 = vtx0 + 1;
	vtx2 = vtx1 + NumBlockXField;
	vtx3 = vtx2 + 1;


	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3DVtxBuffField->Lock(0, 0, (void**)&Vtx, 0);

	if (fx - x > fz - z)
	{
		vec1 = Vtx[vtx1].vtx - Vtx[vtx0].vtx;
		vec2 = Vtx[vtx3].vtx - Vtx[vtx0].vtx;

		D3DXVec3Cross(&cross, &vec1, &vec2);

	}
	else
	{
		vec1 = Vtx[vtx3].vtx - Vtx[vtx0].vtx;
		vec2 = Vtx[vtx2].vtx - Vtx[vtx0].vtx;

		D3DXVec3Cross(&cross, &vec1, &vec2);

	}
	posy = Vtx[vtx0].vtx.y - (cross.x * (no.x - Vtx[vtx0].vtx.x) + cross.z * (no.z - Vtx[vtx0].vtx.z)) / cross.y;

	// 頂点データをアンロックする
	D3DVtxBuffField->Unlock();


	//Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.y = CntVtxZ * 0.0f;
	//Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.z = (NumBlockZField / 2.0f) *  BlockSizeZField - CntVtxZ * BlockSizeZField;

	return (posy);
}

