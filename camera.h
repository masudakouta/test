//=============================================================================
//
// �J�������� [camera.h]
// Author : ���c����
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCamera(void);

D3DXVECTOR3 GetRotCamera(void);
D3DXMATRIX GetMtxView(void);

D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,int Sx,int Sy,float fZ,int Screen_w,int Screen_h,D3DXMATRIX* View,D3DXMATRIX* Prj);

#endif
