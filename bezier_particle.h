//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : ���c�@����
//
//=============================================================================
#ifndef _BEZIER_PARTICLEH_
#define _BEZIER_PARTICLEH_H_

#include "main.h"
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
	int cntFrame;
	D3DXVECTOR3 start;		//
	D3DXVECTOR3 control1;	// �x�W�F�Ȑ��p
	D3DXVECTOR3 control2;	//

} BEZIER_PARTICLEH;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
//BEZIER_PARTICLEH *GetBox_Effect(int pno);
void SetParticle(D3DXVECTOR3 pos);
BEZIER_PARTICLEH *GetParticle(void);
#endif
