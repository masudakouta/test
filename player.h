//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ���c����
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// �v���C���[�\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ���݂̌���
	D3DXVECTOR3 rotDest;	// �ړI�̌���
	float fRadius;			// ���a
	int nIdxShadow;			// �eID
	bool muteki;
} PLAYER;
//*****************************************************************************
// ���@�\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// ���݂̈ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ���݂̌���
	D3DXVECTOR3 rotDest;	// �ړI�̌���
	float fRadius;			// ���a
	int nIdxShadow;			// �eID
} WEAPON;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);
D3DXVECTOR3 GetMovePlayer(void);
D3DXMATRIX *GetPlayerWorldMatrix(void);

#endif
