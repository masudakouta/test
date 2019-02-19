//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 増田光汰
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "camera.h"
#include "meshfield.h"
#include "meshwall.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "life.h"
#include "timer.h"
#include "score.h"
#include "item.h"
#include "box_effect.h"
#include "particle.h"
#include "bezier_particle.h"
#include "score_effect.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

LPDIRECTSOUNDBUFFER8 music;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{

	music = LoadSound(BGM_00);
	// ライトの初期化
	InitLight();

	// カメラの初期化
	InitCamera();

	// 地面の初期化
	InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 200, 200, 10.0f, 10.0f);

	// 壁の初期化
	//InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, 640.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(D3DXVECTOR3(-640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(D3DXVECTOR3(640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -640.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	//// 壁(裏側用)
	//InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, 640.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(D3DXVECTOR3(-640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(D3DXVECTOR3(640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	//InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -640.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//								D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

	// 影の初期化
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();
	// 弾の初期化
	InitBullet();

	// 爆発の初期化
	InitExplosion();

	// エフェクトの初期化
	InitEffect();
	InitBox_Effect();
	InitParticle();
	InitScoreEffect(0);
	// ライフの初期化
	InitLife();

	// タイマーの初期化
	InitTimer();
	ResetTimer(10);

	// スコアの初期化
	InitScore();

	// アイテムの初期化
	InitItem(0);
	for(int nCntCoin = 0; nCntCoin < 50; nCntCoin++)
	{
		float fPosX, fPosY, fPosZ;

		fPosX = (float)(rand() % 1200) / 10.0f - 60.0f;
//		fPosY = (float)(rand() % 1400) / 10.0f + 10.0f;
		fPosY = 40.0f;
		fPosZ = (float)(rand() % 1200) / 10.0f - 60.0f;
		SetItem(D3DXVECTOR3(fPosX, fPosY, fPosZ),  D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_COIN); 
	}

	//InitSound(hWnd);
	// BGM再生
	PlaySound(music, E_DS8_FLAG_LOOP);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	UninitCamera();

	// 地面の終了処理
	UninitMeshField();

	// 壁の終了処理
	UninitMeshWall();

	// 影の終了処理
	UninitShadow();

	// プレイヤーの終了処理
	UninitPlayer();
	// 弾の終了処理
	UninitBullet();

	// 爆発の終了処理
	UninitExplosion();

	// エフェトの終了処理
	UninitEffect();
	UninitBox_Effect();
	UninitParticle();
	UninitScoreEffect();
	// ライフの終了処理
	UninitLife();

	// タイマーの終了処理
	UninitTimer();

	// スコアの終了処理
	UninitScore();

	// アイテムの終了処理
	UninitItem();

	// BGM停止
	//PlaySound(SOUND_LABEL_BGM000);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	// カメラ更新
	UpdateCamera();

	// 地面処理の更新
	UpdateMeshField();

	// 壁処理の更新
	UpdateMeshWall();

	// 影処理の更新
	UpdateShadow();

	// プレイヤー処理の更新
	UpdatePlayer();

	// 弾処理の更新
	UpdateBullet();

	// 爆発処理の更新
	UpdateExplosion();

	// エフェクト処理の更新
	UpdateEffect();
	UpdateBox_Effect();
	UpdateParticle();
	UpdateScoreEffect();
	// ライフ処理の更新
	UpdateLife();

	// タイマー処理の更新
	UpdateTimer();

	// スコア処理の更新
	UpdateScore();

	// アイテム処理の更新
	UpdateItem();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	// 地面処理の描画
	DrawMeshField();

	// 影処理の描画
	DrawShadow();

	// プレイヤー処理の描画
	DrawPlayer();

	// アイテム処理の描画
	DrawItem();

	// 弾処理の描画
	DrawBullet();

	// 壁処理の描画
	DrawMeshWall();

	// 爆発処理の描画
	DrawExplosion();

	// エフェクト処理の描画
	DrawEffect();
	DrawParticle();
	DrawBox_Effect();
	DrawBezier_Particle();
	DrawScoreEffect();
	// ライフ処理の描画
	DrawLife();

	// タイマー処理の描画
	DrawTimer();

	// スコア処理の描画
	DrawScore();
}

