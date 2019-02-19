//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : 
//
//=============================================================================
#ifndef _SCORE_EFFECT_H_
#define _SCORE_EFFECT_H_


// �}�N����`
#define TEXTURE_GAME_SCORE_EFFECT ("data/TEXTURE/s_recover.png")	// �T���v���p�摜
#define TEXTURE_SCORE_EFFECT_SIZE_X	(70)						// �e�N�X�`���T�C�Y
#define TEXTURE_SCORE_EFFECT_SIZE_Y	(70)						// �e�N�X�`���T�C�Y

#define TEXTURE_PATTERN_DIVIDE_X_SCORE_EFFECT	(5)									// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_SCORE_EFFECT	(1)										// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_SCORE_EFFECT			(TEXTURE_PATTERN_DIVIDE_X_SCORE_EFFECT*TEXTURE_PATTERN_DIVIDE_Y_SCORE_EFFECT)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_SCORE_EFFECT			(2)										// �A�j���[�V�����̐؂�ւ��J�E���g

#define SCORE_EFFECT_MAX              (4)	                            //�G�t�F�N�g�p
#define	SCORE_RADIUS_MIN				(50.0f)							// �|���S���̔��a�ŏ��l
#define	RADIUS_MAX				(300.0f)						// �|���S���̔��a�ő�l
#define	SCORE_VALUE_ROTATION			(D3DX_PI * 0.01f)				// �|���S���̉�]��
#define	VALUE_SCORE_EFFECT				(2.0f)							// �|���S���̃X�P�[���ω���
#define	NUM_SCORE_EFFECT				(2)	

//�\���̐錾

typedef struct
{
	VERTEX_2D				vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	D3DXVECTOR3 			pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;					  	// �|���S���̉�]��
	int						CountAnim;					// �A�j���[�V�����J�E���g
	int						PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	bool					use;						//�g�p�A���g�p�̔���
	float					Radius;						// �G�l�~�[�̔��a
	float					BaseAngle;					// �G�l�~�[�̊p�x

}SCOREEFFECT;







//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScoreEffect(int type);
void UninitScoreEffect(void);
void UpdateScoreEffect(void);
void DrawScoreEffect(void);
void SetScoreEffect(D3DXVECTOR3 pos);
SCOREEFFECT *GetScoreEffect(int pno);

#endif