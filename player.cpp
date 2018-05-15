//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
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
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/airplane000.x"	// �ǂݍ��ރ��f����
#define	PLAYER_RADIUS		(10.0f)						// ���a
#define	VALUE_MOVE_PLAYER	(0.155f)					// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.10f)						// ��]�����W��
#define	VALUE_MOVE_BULLET	(7.5f)						// �e�̈ړ����x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer;		// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			g_pMeshPlayer;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXMatBuffPlayer;		// ���b�V���̃}�e���A�������i�[
DWORD				g_nNumMatPlayer;			// �������̑���

D3DXMATRIX			g_mtxWorldPlayer;			// ���[���h�}�g���b�N�X
PLAYER				g_player;					// �v���C���[���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_pD3DTexturePlayer = NULL;
	g_pMeshPlayer = NULL;
	g_pD3DXMatBuffPlayer = NULL;

	g_player.pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.fRadius = PLAYER_RADIUS;

	// X�t�@�C���̓ǂݍ���
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

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,		// �t�@�C���̖��O
								&g_pD3DTexturePlayer);	// �ǂݍ��ރ������[
#endif

	// �e�̐ݒ�
	g_player.nIdxShadow = SetShadow(g_player.pos, g_player.fRadius * 2.0f, g_player.fRadius * 2.0f);		// �e�̐ݒ�

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if(g_pD3DTexturePlayer != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}

	if(g_pMeshPlayer != NULL)
	{// ���b�V���̊J��
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	if(g_pD3DXMatBuffPlayer != NULL)
	{// �}�e���A���̊J��
		g_pD3DXMatBuffPlayer->Release();
		g_pD3DXMatBuffPlayer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 rotCamera;
	float fDiffRotY;

	// �J�����̌����擾
	rotCamera = GetRotCamera();

	if(GetKeyboardPress(DIK_A))
	{
		if(GetKeyboardPress(DIK_W))
		{// ���O�ړ�
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_S))
		{// ����ړ�
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.25f;
		}
		else
		{// ���ړ�
			g_player.move.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y + D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_D))
	{
		if(GetKeyboardPress(DIK_W))
		{// �E�O�ړ�
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_S))
		{// �E��ړ�
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.25f;
		}
		else
		{// �E�ړ�
			g_player.move.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
			g_player.move.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

			g_player.rotDest.y = rotCamera.y - D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_W))
	{// �O�ړ�
		g_player.move.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = D3DX_PI + rotCamera.y;
	}
	else if(GetKeyboardPress(DIK_S))
	{// ��ړ�
		g_player.move.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER;
		g_player.move.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER;

		g_player.rotDest.y = rotCamera.y;
	}

	if(GetKeyboardPress(DIK_UP))
	{// �㏸
		g_player.move.y += VALUE_MOVE_PLAYER;
	}
	if(GetKeyboardPress(DIK_DOWN))
	{// ���~
		g_player.move.y -= VALUE_MOVE_PLAYER;
	}

	if(GetKeyboardPress(DIK_LEFT))
	{// ����]
		g_player.rotDest.y -= VALUE_ROTATE_PLAYER;
		if(g_player.rotDest.y < -D3DX_PI)
		{
			g_player.rotDest.y += D3DX_PI * 2.0f;
		}
	}
	if(GetKeyboardPress(DIK_RIGHT))
	{// �E��]
		g_player.rotDest.y += VALUE_ROTATE_PLAYER;
		if(g_player.rotDest.y > D3DX_PI)
		{
			g_player.rotDest.y -= D3DX_PI * 2.0f;
		}
	}

	// �e����
	if(GetKeyboardTrigger(DIK_SPACE))
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 move;

		pos.x = g_player.pos.x - sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y;
		pos.z = g_player.pos.z - cosf(g_player.rot.y) * 10.0f;

		move.x = -sinf(g_player.rot.y) * VALUE_MOVE_BULLET;
		move.y = 0.0f;
		move.z = -cosf(g_player.rot.y) * VALUE_MOVE_BULLET;

		SetBullet(pos, move, 4.0f, 4.0f, 60 * 4);

		// SE�Đ�
		PlaySound(SOUND_LABEL_SE_SHOT);
	}

	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = g_player.rotDest.y - g_player.rot.y;
	if(fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if(fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if(g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}
	if(g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
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

	// �ړ��ʂɊ�����������
	g_player.move.x += (0.0f - g_player.move.x) * RATE_MOVE_PLAYER;
	g_player.move.y += (0.0f - g_player.move.y) * RATE_MOVE_PLAYER;
	g_player.move.z += (0.0f - g_player.move.z) * RATE_MOVE_PLAYER;

	// �e�̈ʒu�ݒ�
	{
		SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, 0.1f, g_player.pos.z));

		float fSizeX = 20.0f + (g_player.pos.y - 10.0f) * 0.05f;
		if(fSizeX < 20.0f)
		{
			fSizeX = 20.0f;
		}
		float fSizeY = 20.0f + (g_player.pos.y - 10.0f) * 0.05f;
		if(fSizeY < 20.0f)
		{
			fSizeY = 20.0f;
		}

		SetVertexShadow(g_player.nIdxShadow, fSizeX, fSizeY);

		float colA = (200.0f - (g_player.pos.y - 10.0f)) / 400.0f;
		if(colA < 0.0f)
		{
			colA = 0.0f;
		}
		SetColorShadow(g_player.nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, colA));
	}

	if((g_player.move.x * g_player.move.x
	+ g_player.move.y * g_player.move.y
	+ g_player.move.z * g_player.move.z) > 1.0f)
	{
		D3DXVECTOR3 pos;

		pos.x = g_player.pos.x + sinf(g_player.rot.y) * 10.0f;
		pos.y = g_player.pos.y + 2.0f;
		pos.z = g_player.pos.z + cosf(g_player.rot.y) * 10.0f;

		// �G�t�F�N�g�̐ݒ�
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
		SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
	}

	// �A�C�e���Ƃ̓����蔻��
	{
		ITEM *pItem;

		// �A�C�e�����擾
		pItem = GetItem();
		for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
		{
			if(pItem->bUse == true)
			{
				float fLength;

				fLength = (g_player.pos.x - pItem->pos.x) * (g_player.pos.x - pItem->pos.x)
							+ (g_player.pos.y - pItem->pos.y) * (g_player.pos.y - pItem->pos.y)
							+ (g_player.pos.z - pItem->pos.z) * (g_player.pos.z - pItem->pos.z);
				if(fLength < (g_player.fRadius + pItem->fRadius) * (g_player.fRadius + pItem->fRadius))
				{
					// �A�C�e������
					DeleteItem(nCntItem);

					// �X�R�A���Z
					ChangeScore(100);

					// SE�Đ�
					PlaySound(SOUND_LABEL_SE_COIN);
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXMatBuffPlayer->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatPlayer; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		g_pMeshPlayer->DrawSubset(nCntMat);
	}

	{// �}�e���A�����f�t�H���g�ɖ߂�
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// �v���C���[���擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_player.pos;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_player.rot;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_player.rotDest;
}

//=============================================================================
// �ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_player.move;
}
