//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 現在の位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 現在の向き
	D3DXVECTOR3 rotDest;	// 目的の向き
	float fRadius;			// 半径
	int nIdxShadow;			// 影ID
} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
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

#endif
