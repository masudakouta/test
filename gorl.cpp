//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "gorl.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_CAR		 "data/MODEL/gorl.x"							// 読み込むモデル名
#define TEXTURE_FILENAME "data/MODEL/gorl.png"							// テクスチャ

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(1000.0f)										// ビュー平面のFarZ値

#define	POS_CAMERA_P_X	(0.0f)											// カメラ視点の初期位置(X座標)
#define	POS_CAMERA_P_Y	(30.0f)											// カメラ視点の初期位置(Y座標)
#define	POS_CAMERA_P_Z	(-100.0f)										// カメラ視点の初期位置(Z座標)

#define	VALUE_MOVE		(5.0f)											// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 回転量

#define	MOVE_MODEL		(5.0f)

#define	MAX_GORL		(1)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	D3DTextureGorl;				// テクスチャへのポインタ
LPD3DXMESH			D3DXMeshGorl;				// メッシュ情報へのポインタ
LPD3DXBUFFER		D3DXBuffMatGorl;			// マテリアル情報へのポインタ
DWORD				NumMatGorl;					// マテリアル情報の数

D3DXVECTOR3			posGorl;					// モデルの位置
D3DXVECTOR3			rotGorl;					// モデルの向き(回転)
D3DXVECTOR3			sclGorl;					// モデルの大きさ(スケール)

D3DXMATRIX			mtxWorld;					// ワールドマトリックス

static int					IdxShadow;			// 影ID
static float				SizeShadow;			// 影のサイズ
static D3DXCOLOR			colShadow;			// 影の色
bool						fggorl;
float fgRadius;									// 半径

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGorl(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 法線を正規化
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// 位置・回転・スケールの初期設定
	posGorl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotGorl = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	sclGorl = D3DXVECTOR3(1.5f, 1.5f, 1.5f);

	posGorl.x = (float)(rand() % 40 + 1);
	posGorl.z = (float)(rand() % 40 + 1);
	fggorl = false;
	D3DTextureGorl = NULL;
	D3DXMeshGorl = NULL;
	D3DXBuffMatGorl = NULL;
	NumMatGorl = 0;

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX/*初期化もしてくれる*/(PLAYER_CAR,		// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		Device,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatGorl,					// マテリアルデータを含むバッファへのポインタ　材質情報
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatGorl,						// D3DXMATERIAL構造体の数　何個のマテリルがあるか
		&D3DXMeshGorl)))					// ID3DXMeshインターフェイスへのポインタのアドレス　
	{
		return E_FAIL;
	}

#if 1
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,									// デバイスへのポインタ
		TEXTURE_FILENAME,						// ファイルの名前
		&D3DTextureGorl);					// 読み込むメモリー
#endif



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGorl(void)
{
	if (D3DTextureGorl != NULL)
	{// テクスチャの開放
		D3DTextureGorl->Release();
		D3DTextureGorl = NULL;
	}

	if (D3DXMeshGorl != NULL)
	{// メッシュの開放
		D3DXMeshGorl->Release();
		D3DXMeshGorl = NULL;
	}

	if (D3DXBuffMatGorl != NULL)
	{// マテリアルの開放
		D3DXBuffMatGorl->Release();
		D3DXBuffMatGorl = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGorl(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	//if (pPlayer->pos.x == posGorl.x)
	//{
	//	SetFade(FADE_OUT);

	//}
	for (int nCntItem = 0; nCntItem < MAX_GORL; nCntItem++)
	{
		if (fggorl == true)
		{
			float fLength;

			fLength = (pPlayer->pos.x - posGorl.x) * (pPlayer->pos.x - posGorl.x)
				+ (pPlayer->pos.y - posGorl.y) * (pPlayer->pos.y - posGorl.y)
				+ (pPlayer->pos.z - posGorl.z) * (pPlayer->pos.z - posGorl.z);
			if (fLength < (pPlayer->fRadius + fgRadius) * (pPlayer->fRadius + fgRadius))
			{

				SetFade(FADE_OUT);

				// SE再生
				//PlaySound(SOUND_LABEL_SE_COIN);
			}
		}

	}

	posGorl.y = Get(posGorl);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGorl(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, sclGorl.x, sclGorl.y, sclGorl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotGorl.y, rotGorl.x, rotGorl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, posGorl.x, posGorl.y, posGorl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得//元の戻すためのバックアップ
	Device->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	D3DXMat = (D3DXMATERIAL*)D3DXBuffMatGorl->GetBufferPointer();

	//マテリアルの数だけ回す
	for (int CntMat = 0; CntMat < (int)NumMatGorl; CntMat++)
	{
		// マテリアルの設定
		Device->SetMaterial(&D3DXMat[CntMat].MatD3D);

		// テクスチャの設定
		Device->SetTexture(0
			, D3DTextureGorl);

		// 描画
		//D3DXMeshGorl->DrawSubset(CntMat);
	}

	// マテリアルをデフォルトに戻す
	Device->SetMaterial(&matDef);
}

//=============================================================================
// エネミーの取得関数
//=============================================================================

D3DXVECTOR3 *GetGorlPos(void)
{
	return &posGorl;
}
//=============================================================================
// プレイヤーとエネミーの当たり判定
//=============================================================================

//void CollisionDetection(void)
//{
//	for (int K = 0; K < ENEMY_MAX; K++)
//	{
//		for (int l = 0; l < PLAYER_MAX; l++)
//		{
//
//			if ((enemy->use == true) && (player->use == true))
//			{
//
//				if ((GetPlayerPos() > posGorl.x) &&
//					(player->pos.x - 20 * 0.72f < enemy->pos.x + 20))
//				{
//					if ((player->pos.y + 20 > enemy->pos.y - 20) &&
//						(player->pos.y - 20 < enemy->pos.y + 20))
//					{
//						SetEffect(enemy->pos);
//						break;
//					}
//				}
//			}
//		}
//	}
//}
