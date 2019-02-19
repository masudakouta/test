//=============================================================================
//
// �J�������� [camera.cpp]
// Author : ���c�@����
//
//=============================================================================
#include "camera.h"
#include "player.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// ����p
#define	VIEW_NEAR_Z			(10.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(5000.0f)				// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)					// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// �J�����̉�]��

#define	INTERVAL_CAMERA_R	(12.5f)					// ���f���̎����̐�܂ł̋���
#define	RATE_CHASE_CAMERA_P	(0.35f)					// �J�����̎��_�ւ̕␳�W��
#define	RATE_CHASE_CAMERA_R	(0.20f)					// �J�����̒����_�ւ̕␳�W��

#define	CHASE_HEIGHT_P		(100.0f)				// �ǐՎ��̎��_�̍���
#define	CHASE_HEIGHT_R		(10.0f)					// �ǐՎ��̒����_�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3		g_posCameraP;				// �J�����̎��_
D3DXVECTOR3		g_posCameraR;				// �J�����̒����_
D3DXVECTOR3		g_posCameraU;				// �J�����̏����
D3DXVECTOR3		g_posCameraPDest;			// �J�����̎��_�̖ړI�ʒu
D3DXVECTOR3		g_posCameraRDest;			// �J�����̒����_�̖ړI�ʒu
D3DXVECTOR3		g_rotCamera;				// �J�����̉�]
float			g_fLengthIntervalCamera;	// �J�����̎��_�ƒ����_�̋���
float			g_fLengthIntervalPlayer;	// �v���C���[�ƒ����_�̋���
D3DXMATRIX		g_mtxView;					// �r���[�}�g���b�N�X
D3DXMATRIX		g_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

//=============================================================================
// �J�����̏�����
//=============================================================================
HRESULT InitCamera(void)
{
	g_posCameraP = D3DXVECTOR3(0.0f, 30.0f, -150.0f);
	g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_posCameraPDest = D3DXVECTOR3(0.0f, 10.0f, -200.0f);
	g_posCameraRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float vx,vz;
	vx = g_posCameraP.x - g_posCameraR.x;
	vz = g_posCameraP.z - g_posCameraR.z;
	g_fLengthIntervalCamera = sqrtf(vx * vx + vz * vz);
	g_fLengthIntervalPlayer = INTERVAL_CAMERA_R;

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	D3DXVECTOR3 posPlayer;
	D3DXVECTOR3 rotPlayer;
	D3DXVECTOR3 movePlayer;
	float fLength;

	// ���f���̈ʒu�擾
	posPlayer = GetPositionPlayer();

	// ���f���̖ړI�̌����擾
	rotPlayer = GetRotationPlayer();

	// ���f���̈ړ��ʎ擾
	movePlayer = GetMovePlayer();

	// ���_�̖ړI�ʒu
	fLength = sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	g_posCameraPDest.x = posPlayer.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera - sin(rotPlayer.y) * fLength;
	g_posCameraPDest.y = posPlayer.y + CHASE_HEIGHT_P;
	g_posCameraPDest.z = posPlayer.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera - cos(rotPlayer.y) * fLength;

	// �����_�̖ړI�ʒu
	fLength = g_fLengthIntervalPlayer + sqrtf(movePlayer.x * movePlayer.x + movePlayer.z * movePlayer.z) * 6.0f;
	g_posCameraRDest.x = posPlayer.x - sin(rotPlayer.y) * fLength;
	g_posCameraRDest.y = posPlayer.y - CHASE_HEIGHT_R;
	g_posCameraRDest.z = posPlayer.z - cos(rotPlayer.y) * fLength;

	// ���_�̕␳
	g_posCameraP.x += (g_posCameraPDest.x - g_posCameraP.x) * RATE_CHASE_CAMERA_P;
	g_posCameraP.y += (g_posCameraPDest.y - g_posCameraP.y) * RATE_CHASE_CAMERA_P;
	g_posCameraP.z += (g_posCameraPDest.z - g_posCameraP.z) * RATE_CHASE_CAMERA_P;
	//g_posCameraP.x = g_posCameraR.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//g_posCameraP.y = g_posCameraR.y + 50.0f/*sinf(rotCamera.y) * fLengthIntervalCamera*/;
	//g_posCameraP.z = g_posCameraR.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera;

	// �����_�̕␳
	g_posCameraR.x += (g_posCameraRDest.x - g_posCameraR.x) * RATE_CHASE_CAMERA_R;
	g_posCameraR.y += (g_posCameraRDest.y - g_posCameraR.y) * RATE_CHASE_CAMERA_R;
	g_posCameraR.z += (g_posCameraRDest.z - g_posCameraR.z) * RATE_CHASE_CAMERA_R;
	//if (GetKeyboardPress(DIK_RIGHT))
	//{// ���_����u���v
	//	g_rotCamera.y += VALUE_ROTATE_CAMERA;
	//	if (g_rotCamera.y > D3DX_PI)
	//	{
	//		g_rotCamera.y -= D3DX_PI * 2.0f;
	//	}

	//	g_posCameraP.x = g_posCameraR.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraP.z = g_posCameraR.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}
	//if (GetKeyboardPress(DIK_LEFT))
	//{// ���_����u�E�v
	//	g_rotCamera.y -= VALUE_ROTATE_CAMERA;
	//	if (g_rotCamera.y < -D3DX_PI)
	//	{
	//		g_rotCamera.y += D3DX_PI * 2.0f;
	//	}

	//	g_posCameraP.x = g_posCameraR.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraP.z = g_posCameraR.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}

	if (GetKeyboardPress(DIK_9))
	{
		g_posCameraP.z += 1.0f;
	}
	if (GetKeyboardPress(DIK_0))
	{// �����_�Ǝ��_�Ƃ̋���
		g_rotCamera.x -= VALUE_ROTATE_CAMERA;
		//g_rotCamera.y += 0.02f;
		if (g_rotCamera.x < -D3DX_PI)
		{
			g_rotCamera.x += D3DX_PI * 2.0f;
		}
		g_posCameraP.y = g_posCameraR.y - sinf(g_rotCamera.x) * g_fLengthIntervalCamera;
		g_posCameraP.z = g_posCameraR.z - cosf(g_rotCamera.x) * g_fLengthIntervalCamera;

	}

}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_posCameraP,		// �J�����̎��_
						&g_posCameraR,		// �J�����̒����_
						&g_posCameraU);		// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								VIEW_ANGLE,										// ����p
								((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// �A�X�y�N�g��
								VIEW_NEAR_Z,									// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);									// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return g_rotCamera;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_mtxView;
}
//=============================================================================
// �X�N���[�����W�����[���h���W�ɕϊ� �֐�
//=============================================================================
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,
								int Sx,			// �X�N���[��X���W
								int Sy,			// �X�N���[��Y���W
								float fZ,		// �ˉe��Ԃł�Z�l�i0�`1�j
								int Screen_w,
								int Screen_h,
								D3DXMATRIX* View,
								D3DXMATRIX* Prj
								)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return pout;
}
