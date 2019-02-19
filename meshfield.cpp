//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : ���c����
//
//=============================================================================
#include "meshfield.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/field000.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 D3DTextureField;			// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 D3DIdxBuffField;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX mtxWorldField;						// ���[���h�}�g���b�N�X
D3DXVECTOR3 posField;							// �|���S���\���ʒu�̒��S���W
D3DXVECTOR3 rotField;							// �|���S���̉�]�p

int NumBlockXField, NumBlockZField;		// �u���b�N��
int NumVertexField;							// �����_��	
int NumVertexIndexField;						// ���C���f�b�N�X��
int NumPolygonField;							// ���|���S����
float BlockSizeXField, BlockSizeZField;	// �u���b�N�T�C�Y
float Xrand, Zrand, Rrand;
float valField;
float xMax, zMax, rMax;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	xMax = (fBlockSizeX * nNumBlockX) / 2;
	zMax = (fBlockSizeX * nNumBlockX) / 2;
	rMax = 100;

	valField = (fBlockSizeX * nNumBlockX);

	// �|���S���\���ʒu�̒��S���W��ݒ�
	posField = pos;

	rotField = rot;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,		// �t�@�C���̖��O
								&D3DTextureField);	// �ǂݍ��ރ������[

	// �u���b�N���̐ݒ�
	NumBlockXField = nNumBlockX;
	NumBlockZField = nNumBlockZ;

	// ���_���̐ݒ�
	NumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	NumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	NumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	BlockSizeXField = fBlockSizeX;
	BlockSizeZField = fBlockSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffField,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED( pDevice->CreateIndexBuffer(sizeof(WORD) * NumVertexIndexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DIdxBuffField,					// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < (NumBlockZField + 1); nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (NumBlockXField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].vtx.x = -(NumBlockXField / 2.0f) * BlockSizeXField + nCntVtxX * BlockSizeXField;
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].vtx.z = (NumBlockZField / 2.0f) * BlockSizeZField - nCntVtxZ * BlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// ���ˌ��̐ݒ�
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (NumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffField->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DIdxBuffField->Lock( 0, 0, (void**)&pIdx, 0 );

		int nCntIdx = 0;
		for(int nCntVtxZ = 0; nCntVtxZ < NumBlockZField; nCntVtxZ++)
		{
			if(nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
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
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (NumBlockXField + 1) + NumBlockXField;
				nCntIdx++;
			}
		}
#if 1
		// �t���N�^�����_�@�f�w�t���N�^���@�����_���n�`����
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
		// �C���f�b�N�X�f�[�^���A�����b�N����
		D3DIdxBuffField->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if(D3DVtxBuffField)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffField->Release();
		D3DVtxBuffField = NULL;
	}

	if(D3DIdxBuffField)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		D3DIdxBuffField->Release();
		D3DIdxBuffField = NULL;
	}

	if(D3DTextureField)
	{// �e�N�X�`���̊J��
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

	// �t���N�^�����_�@�f�w�t���N�^���@�����_���n�`����
	if (GetKeyboardPress(DIK_X))
	{

	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorldField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotField.y, rotField.x, rotField.z);
	D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posField.x, posField.y, posField.z);
	D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldField);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(D3DIdxBuffField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, D3DTextureField);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertexField, 0, NumPolygonField);
}
//=============================================================================
// �擾�֐�
//=============================================================================

float Get(D3DXVECTOR3 no)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
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


	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
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

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffField->Unlock();


	//Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.y = CntVtxZ * 0.0f;
	//Vtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.z = (NumBlockZField / 2.0f) *  BlockSizeZField - CntVtxZ * BlockSizeZField;

	return (posy);
}

