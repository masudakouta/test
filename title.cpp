//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : 増田光汰
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_TITLE		"data/MODEL/title.x"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE	"data/TEXTURE/title.png"	// 読み込むテクスチャファイル名
#define TEXTURE_LOGO_START	"data/TEXTURE/coollogo_com-3677941.png"

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(1000.0f)										// ビュー平面のFarZ値
#define	POS_CAMERA_P_X	(0.0f)											// カメラ視点の初期位置(X座標)
#define	POS_CAMERA_P_Y	(30.0f)											// カメラ視点の初期位置(Y座標)
#define	POS_CAMERA_P_Z	(-50.0f)										// カメラ視点の初期位置(Z座標)

//#define	TITLE_LOGO_POS_X		(220)		// タイトルロゴの位置(X座標)
//#define	TITLE_LOGO_POS_Y		(40)		// タイトルロゴの位置(Y座標)
//#define	TITLE_LOGO_WIDTH		(940)		// タイトルロゴの幅
//#define	TITLE_LOGO_HEIGHT		(640)		// タイトルロゴの高さ

#define	START_POS_X				(400)		// スタートボタンの位置(X座標)
#define	START_POS_Y				(720)		// スタートボタンの位置(Y座標)
#define	START_WIDTH				(480)		// スタートボタンの幅
#define	START_HEIGHT			(120)		// スタートボタンの高さ

#define	COUNT_APPERA_START		(60)		// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(60)		// スタートボタン点滅の時間

#define	COUNT_WAIT_DEMO			(60 * 5)	// デモまでの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
//void SetColorTitleLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int						g_nCountAppearStart = 0;		//
float					g_fAlphaLogo = 0.0f;			//
int						g_nCountDisp = 0;				//
bool					g_bDispStart = false;			//
int						g_nConutDemo = 0;				//

LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// 頂点バッファインターフェースへのポインタ
//LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// テクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// 頂点バッファインターフェースへのポインタ
LPD3DXMESH				g_pD3DXMeshTitle;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER			g_pD3DXMatBuffTitle;			// メッシュのマテリアル情報を格納
DWORD					g_nNumMatTitle;					// 属性情報の総数

D3DXMATRIX				g_mtxWorldTitle;				// ワールドマトリックス

D3DXVECTOR3			posTitle;							// モデルの位置
D3DXVECTOR3			rotTitle;							// モデルの向き(回転)
D3DXVECTOR3			sclTitle;							// モデルの大きさ(スケール)

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearStart = 0;
	g_fAlphaLogo = 0.0f;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_nConutDemo = 0;

	// 法線を正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// 位置・回転・スケールの初期設定
	posTitle = D3DXVECTOR3(0.0f, 90.0f, -100.0f);
	rotTitle = D3DXVECTOR3(0.0f, -20.0f, -0.5f);
	sclTitle = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	g_pD3DTextureTitle = NULL;
	g_pD3DXMeshTitle = NULL;
	g_pD3DXMatBuffTitle = NULL;
	g_nNumMatTitle = 0;

	// 頂点情報の作成
	MakeVertexTitle(pDevice);


	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_LOGO_START,			// ファイルの名前
								&g_pD3DTextureStart);		// 読み込むメモリー
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX/*初期化もしてくれる*/(MODEL_TITLE,		// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,						// メッシュの作成オプションを指定
		pDevice,								// IDirect3DDevice9インターフェイスへのポインタ
		NULL,									// 隣接性データを含むバッファへのポインタ
		&g_pD3DXMatBuffTitle,					// マテリアルデータを含むバッファへのポインタ　材質情報
		NULL,									// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMatTitle,						// D3DXMATERIAL構造体の数　何個のマテリルがあるか
		&g_pD3DXMeshTitle)))					// ID3DXMeshインターフェイスへのポインタのアドレス　
	{
		return E_FAIL;
	}
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_TITLE,			// ファイルの名前
								&g_pD3DTextureTitle);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
//	if(g_pD3DTextureTitle != NULL)
//	{// テクスチャの開放
//		g_pD3DTextureTitle->Release();
//		g_pD3DTextureTitle = NULL;
//	}
//
//	if(g_pD3DVtxBuffTitle != NULL)
//	{// 頂点バッファの開放
//		g_pD3DVtxBuffTitle->Release();
//		g_pD3DVtxBuffTitle = NULL;
//	}
//
	//if(g_pD3DTextureTitleLogo != NULL)
	//{// テクスチャの開放
	//	g_pD3DTextureTitleLogo->Release();
	//	g_pD3DTextureTitleLogo = NULL;
	//}

	//if(g_pD3DVtxBuffTitleLogo != NULL)
	//{// 頂点バッファの開放
	//	g_pD3DVtxBuffTitleLogo->Release();
	//	g_pD3DVtxBuffTitleLogo = NULL;
//	}
//
	if(g_pD3DTextureStart != NULL)
	{// テクスチャの開放
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if(g_pD3DVtxBuffStart != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}

	if (g_pD3DTextureTitle != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if (g_pD3DXMeshTitle != NULL)
	{// メッシュの開放
		g_pD3DXMeshTitle->Release();
		g_pD3DXMeshTitle = NULL;
	}

	if (g_pD3DXMatBuffTitle != NULL)
	{// マテリアルの開放
		g_pD3DXMatBuffTitle->Release();
		g_pD3DXMatBuffTitle = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
#if 0
	if(g_nCountAppearStart >= COUNT_APPERA_START)
	{
		g_nConutDemo++;
		if(g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
#endif
	D3DXVECTOR3 rotCamera;

	rotCamera = GetRotCamera();


	if(g_fAlphaLogo < 1.0f)
	{
		g_fAlphaLogo += 0.005f;
		if(g_fAlphaLogo >= 1.0f)
		{
			g_fAlphaLogo = 1.0f;
		}
		//SetColorTitleLogo();
	}
	else
	{
		g_nCountAppearStart++;
		if(g_nCountAppearStart > COUNT_APPERA_START)
		{
			g_nCountDisp = (g_nCountDisp + 1) % 80;
			if(g_nCountDisp > INTERVAL_DISP_START)
			{
				g_bDispStart = false;
			}
			else
			{
				g_bDispStart = true;
			}
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{
		if(g_nCountAppearStart == 0)
		{// タイトル登場スキップ
			g_fAlphaLogo = 1.0f;
			//SetColorTitleLogo();

			g_nCountAppearStart = COUNT_APPERA_START;
		}
		else
		{// ゲームへ
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXMATRIX mtxRot, mtxTranslate;
	//D3DXMATERIAL *pD3DXMat;


	//// 頂点バッファをデバイスのデータストリームにバインド
 //   pDevice->SetStreamSource(0, g_pD3DVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_pD3DTextureTitleLogo);

	//// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	//if(g_bDispStart == true)
	//{
		// 頂点バッファをデバイスのデータストリームにバインド
	

	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *D3DXMat;
	D3DMATERIAL9 matDef;
		Device->SetStreamSource(0, g_pD3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, g_pD3DTextureStart);

		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldTitle);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, sclTitle.x, sclTitle.y, sclTitle.z);
	//D3DXMatrixScaling(&mtxScl, 0.1f,0.1f,0.1f);

	D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTitle.y, rotTitle.x, rotTitle.z);
	D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, posTitle.x, posTitle.y, posTitle.z);
	D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &g_mtxWorldTitle);

	// 現在のマテリアルを取得//元の戻すためのバックアップ
	Device->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	D3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffTitle->GetBufferPointer();

	//マテリアルの数だけ回す
	for (int nCntMat = 0; nCntMat < (int)g_nNumMatTitle; nCntMat++)
	{
		// マテリアルの設定
		Device->SetMaterial(&D3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		Device->SetTexture(0, g_pD3DTextureTitle);

		// 描画
		g_pD3DXMeshTitle->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	Device->SetMaterial(&matDef);



	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&g_mtxWorldTitle);

	//// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTitle.y, rotTitle.x, rotTitle.z);
	//D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxRot);

	//// 移動を反映
	//D3DXMatrixTranslation(&mtxTranslate, posTitle.x, posTitle.y, posTitle.z);
	//D3DXMatrixMultiply(&g_mtxWorldTitle, &g_mtxWorldTitle, &mtxTranslate);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldTitle);

	//// マテリアル情報に対するポインタを取得
	//pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffTitle->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_nNumMatTitle; nCntMat++)
	//{
	//	// マテリアルの設定
	//	pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

	//	// テクスチャの設定
	//	pDevice->SetTexture(0, g_pD3DTextureTitle);

	//	// 描画
	//	g_pD3DXMeshTitle->DrawSubset(nCntMat);
	//}

	//{// マテリアルをデフォルトに戻す
	//	D3DXMATERIAL mat;

	//	mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//	mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//	mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//	pDevice->SetMaterial(&mat.MatD3D);
	//}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
 //   if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
	//											D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
	//											FVF_VERTEX_2D,				// 使用する頂点フォーマット
	//											D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
	//											&g_pD3DVtxBuffTitle,		// 頂点バッファインターフェースへのポインタ
	//											NULL)))						// NULLに設定
	//{
 //       return E_FAIL;
	//}
//
//	{//頂点バッファの中身を埋める
//		VERTEX_2D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);
//
//		// 頂点座標の設定
//		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
//
//		// テクスチャのパースペクティブコレクト用
//		pVtx[0].rhw =
//		pVtx[1].rhw =
//		pVtx[2].rhw =
//		pVtx[3].rhw = 1.0f;
//
//		// 反射光の設定
//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		// テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffTitle->Unlock();
//	}
//
//	// オブジェクトの頂点バッファを生成
//    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
//												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
//												FVF_VERTEX_2D,				// 使用する頂点フォーマット
//												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
//												&g_pD3DVtxBuffTitleLogo,	// 頂点バッファインターフェースへのポインタ
//												NULL)))						// NULLに設定
//	{
//        return E_FAIL;
//	}
//
//	{//頂点バッファの中身を埋める
//		VERTEX_2D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);
//
//		// 頂点座標の設定
//		pVtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y, 0.0f);
//		pVtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y, 0.0f);
//		pVtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
//		pVtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
//
//		// テクスチャのパースペクティブコレクト用
//		pVtx[0].rhw =
//		pVtx[1].rhw =
//		pVtx[2].rhw =
//		pVtx[3].rhw = 1.0f;
//
//		// 反射光の設定
//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//
//		// テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffTitleLogo->Unlock();
//	}
//
//
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffStart,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}
//
	//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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

		// 頂点データをアンロックする
		g_pD3DVtxBuffStart->Unlock();
	

	return S_OK;
}
//
////=============================================================================
//// 頂点の作成
////=============================================================================
//void SetColorTitleLogo(void)
//{
//	{//頂点バッファの中身を埋める
//		VERTEX_2D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);
//
//		// 反射光の設定
//		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
//
//		// 頂点データをアンロックする
//		g_pD3DVtxBuffTitleLogo->Unlock();
//	}
//
//}
//
