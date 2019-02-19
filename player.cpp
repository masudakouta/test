//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 増田光汰
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "item.h"
#include "score.h"
#include "meshfield.h"
#include "fade.h"
#include "box_effect.h"
#include "particle.h"
#include "bezier_particle.h"
#include "score_effect.h"
#include "explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//モデル
#define	MODEL_PLAYER		"data/PLAYER/nitousinn.x"	// 読み込むモデル名
#define TEXTURE_FILENAME	"data/PLAYER/kao.png"		//テクスチャ
#define	PLAYER_RADIUS		(10.0f)						// 半径
#define	VALUE_MOVE_PLAYER	(0.155f)					// 移動速度
#define	RATE_MOVE_PLAYER	(0.125f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.10f)						// 回転慣性係数
#define	VALUE_MOVE_BULLET	(7.5f)						// 弾の移動速度

//剣
#define PLAYER_WEAPON		"data/PLAYER/weapon.x"		// 読み込むモデル名
#define TEXTURE_WEAPON		"data/PLAYER/weapon.png"	// 読み込むモデル名
#define WEAPON_RADIUS		(20.0f)						// 半径
#define	VALUE_ROTATE_WEAPON	(D3DX_PI * 5.025f)			// 回転速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawPlayer_Sword(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//モデル
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer;		// テクスチャ読み込み場所
LPD3DXMESH			g_pMeshPlayer;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		g_pD3DXMatBuffPlayer;		// メッシュのマテリアル情報を格納
DWORD				g_nNumMatPlayer;			// 属性情報の総数
//剣
LPDIRECT3DTEXTURE9	D3DTextureWeapon;		// テクスチャ読み込み場所
LPD3DXMESH			MeshWeapon;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXMatBuffWeapon;		// メッシュのマテリアル情報を格納
DWORD				NumMatWeapon;			// 属性情報の総数
float				fLengthIntervalWeapon;
D3DXMATRIX			g_mtxWorldPlayer;			// ワールドマトリックス
D3DXMATRIX			mtxWorldWeapon;				// 右ワールドマトリックス

PLAYER				g_player;					// プレイヤーワーク
WEAPON				weapon;						// ウェポンワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スケールのなんやかんや
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//モデルの初期化
	g_pD3DTexturePlayer = NULL;
	g_pMeshPlayer = NULL;
	g_pD3DXMatBuffPlayer = NULL;
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.fRadius = PLAYER_RADIUS;

	//武器の初期化
	D3DTextureWeapon = NULL;
	MeshWeapon = NULL;
	D3DXMatBuffWeapon = NULL;
	weapon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	weapon.rot = D3DXVECTOR3(3.4f, 0.0f, 0.0f);
	weapon.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	weapon.fRadius = WEAPON_RADIUS;



	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromX(MODEL_PLAYER,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_pD3DXMatBuffPlayer,
								NULL,
								&g_nNumMatPlayer,
								&g_pMeshPlayer)))
	{
		return E_FAIL;
	}
	// 剣のXファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX/*初期化もしてくれる*/(PLAYER_WEAPON,// 読み込むモデルファイル名(Xファイル)
								D3DXMESH_SYSTEMMEM,					// メッシュの作成オプションを指定
								pDevice,							// IDirect3DDevice9インターフェイスへのポインタ
								NULL,								// 隣接性データを含むバッファへのポインタ
								&D3DXMatBuffWeapon,					// マテリアルデータを含むバッファへのポインタ　材質情報
								NULL,								// エフェクトインスタンスの配列を含むバッファへのポインタ
								&NumMatWeapon,						// D3DXMATERIAL構造体の数　何個のマテリルがあるか
								&MeshWeapon)))						// ID3DXMeshインターフェイスへのポインタのアドレス　
	{
		return E_FAIL;
	}

#if 1
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&g_pD3DTexturePlayer);	// 読み込むメモリー
														// テクスチャの読み込み
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_WEAPON,			// ファイルの名前
								&D3DTextureWeapon);		// 読み込むメモリー

#endif

	// 影の設定
	g_player.nIdxShadow = SetShadow(g_player.pos,
	g_player.fRadius * 2.0f, g_player.fRadius * 2.0f);	// 影の設定

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	//////////
	//モデル//
	//////////
	if(g_pD3DTexturePlayer != NULL)
	{// テクスチャの開放
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}

	if(g_pMeshPlayer != NULL)
	{// メッシュの開放
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	if(g_pD3DXMatBuffPlayer != NULL)
	{// マテリアルの開放
		g_pD3DXMatBuffPlayer->Release();
		g_pD3DXMatBuffPlayer = NULL;
	}
	//////
	//剣//
	//////
	if (D3DTextureWeapon != NULL)
	{// テクスチャの開放
		D3DTextureWeapon->Release();
		D3DTextureWeapon = NULL;
	}

	if (MeshWeapon != NULL)
	{// メッシュの開放
		MeshWeapon->Release();
		MeshWeapon = NULL;
	}

	if (D3DXMatBuffWeapon != NULL)
	{// マテリアルの開放
		D3DXMatBuffWeapon->Release();
		D3DXMatBuffWeapon = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 rotCamera;
	float fDiffRotY;

	// カメラの向き取得
	rotCamera = GetRotCamera();

	if(GetKeyboardPress(DIK_A))
	{
		if(GetKeyboardPress(DIK_W))
		{// 左前移動
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_S))
		{// 左後移動
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.25f;
		}
		else
		{// 左移動
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_D))
	{
		if(GetKeyboardPress(DIK_W))
		{// 右前移動
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;


			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_S))
		{// 右後移動
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.25f;
		}
		else
		{// 右移動
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_W))
	{// 前移動
		g_player.move.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = D3DX_PI + rotCamera.y;

	}
	else if(GetKeyboardPress(DIK_S))
	{// 後移動
		g_player.move.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = rotCamera.y;
	}


	// 弾発射
	if(GetKeyboardTrigger(DIK_RETURN))
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 move;
		//SetFade(FADE_OUT);

		pos.x = g_player.pos.x - sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y;
		pos.z = g_player.pos.z - cosf(g_player.rot.y) * 10.0f;

		move.x = -sinf(g_player.rot.y) * VALUE_MOVE_BULLET;
		move.y = 0.0f;
		move.z = -cosf(g_player.rot.y) * VALUE_MOVE_BULLET;

		SetBullet(pos, move, 4.0f, 4.0f, 60 * 4);

		// SE再生
		//PlaySound(SOUND_LABEL_SE_SHOT);
	}

	// 目的の角度までの差分
	fDiffRotY = g_player.rotDest.y - g_player.rot.y;
	if(fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if(fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	g_player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if(g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}
	if(g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}


	weapon.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (weapon.rot.y > D3DX_PI)
	{
		weapon.rot.y -= D3DX_PI * 2.0f;
	}
	if (weapon.rot.y < -D3DX_PI)
	{
		weapon.rot.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	if(g_player.pos.x < -630.0f)
	{
		g_player.pos.x = -630.0f;
	}
	if(g_player.pos.x > 630.0f)
	{
		g_player.pos.x = 630.0f;
	}
	if(g_player.pos.y < 10.0f)
	{
		g_player.pos.y = 10.0f;
	}
	if(g_player.pos.y > 150.0f)
	{
		g_player.pos.y = 150.0f;
	}
	if(g_player.pos.z > 630.0f)
	{
		g_player.pos.z = 630.0f;
	}
	if(g_player.pos.z < -630.0f)
	{
		g_player.pos.z = -630.0f;
	}

	// 移動量に慣性をかける
	g_player.move.x += (0.0f - g_player.move.x) * RATE_MOVE_PLAYER;
	g_player.move.y += (0.0f - g_player.move.y) * RATE_MOVE_PLAYER;
	g_player.move.z += (0.0f - g_player.move.z) * RATE_MOVE_PLAYER;

	weapon.move.x += (0.0f - weapon.move.x) * RATE_MOVE_PLAYER;
	weapon.move.y += (0.0f - weapon.move.y) * RATE_MOVE_PLAYER;
	weapon.move.z += (0.0f - weapon.move.z) * RATE_MOVE_PLAYER;


	if((g_player.move.x * g_player.move.x
	+ g_player.move.y * g_player.move.y
	+ g_player.move.z * g_player.move.z) > 1.0f)
	{
		D3DXVECTOR3 pos;

		pos.x = g_player.pos.x + sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y + 2.0f;
		pos.z = g_player.pos.z + cosf(g_player.rot.y) * 10.0f;

		// エフェクトの設定
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.50f), 14.0f, 14.0f, 20);
		//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//					D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
		//SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//					D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
	}

	// エネミーとの当たり判定
	{
		ITEM *pItem;
		BOX_PARTICLE *Box_Effect = GetBox_Effect(0);
		PARTICLE *Particle = GetParticlet(0);

		float fLength;

		pItem = GetItem();
		if (GetKeyboardTrigger(DIK_P))
		{
				weapon.rot.x -= 15.0f;
			for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
			{
				if (pItem->bUse == true)
				{
					float fLength;

					fLength = (weapon.worldPos.x - pItem->pos.x) * (weapon.worldPos.x - pItem->pos.x)
						+ (weapon.worldPos.y - pItem->pos.y) * (weapon.worldPos.y - pItem->pos.y) * 2
						+ (weapon.worldPos.z - pItem->pos.z) * (weapon.worldPos.z - pItem->pos.z);
					if (fLength < (weapon.fRadius + pItem->fRadius) * (weapon.fRadius + pItem->fRadius))
					{
						for (int i = 0; i < 30; i++, Box_Effect++,Particle++)
						{
							SetBox_Effect(pItem->pos, Box_Effect->move, D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f), 20.0f, 20.0f, 100);
							SetParticle(pItem->pos, Particle->move, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 20.0f, 20.0f, 50);
							SetBezier_Particle(pItem->pos);
							SetExplosion(pItem->pos, 10.0f, 10.0f);
						}

						// スコア加算
						pItem->bUse = false;
						// SE再生
						//PlaySound(SOUND_LABEL_SE_COIN);
					}
				}
			}
		}
		if (GetKeyboardRelease(DIK_P))
		{
			weapon.rot.x += 15.0f;

		}

	}
	g_player.pos.y = Get(g_player.pos);
	weapon.pos.y = Get(weapon.pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffPlayer->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatPlayer; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		g_pMeshPlayer->DrawSubset(nCntMat);
	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
		//子の処理
		DrawPlayer_Sword();

	}
}
//=============================================================================
//剣 描画処理
//=============================================================================


void DrawPlayer_Sword(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorldWeapon);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, weapon.rot.x, 0.0, weapon.rot.z);
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 2.6f, -2.0f);
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, &mtxTranslate);


	//胴体(親)のワールドマトリクスのワールドマトリクスをかける
	D3DXMatrixMultiply(&mtxWorldWeapon, &mtxWorldWeapon, GetPlayerWorldMatrix());
	D3DXVec3TransformCoord(&weapon.worldPos, &weapon.pos, &mtxWorldWeapon);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldWeapon);

	// 現在のマテリアルを取得//元の戻すためのバックアップ
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXMatBuffWeapon->GetBufferPointer();

	//マテリアルの数だけ回す
	for (int nCntMat = 0; nCntMat < (int)NumMatWeapon; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0
			, D3DTextureWeapon);

		// 描画
		MeshWeapon->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// プレイヤーを取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_player.pos;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_player.rot;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_player.rotDest;
}

//=============================================================================
// 移動量取得
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_player.move;
}
//=============================================================================
// モデル(親)のワールドマトリックス取得
//=============================================================================
D3DXMATRIX *GetPlayerWorldMatrix(void)
{
	return &g_mtxWorldPlayer;
}
