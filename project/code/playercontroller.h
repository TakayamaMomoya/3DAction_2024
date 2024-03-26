//*****************************************************
//
// �v���C���[����[playercontroller.h]
// Author:���R����
//
//*****************************************************

#ifndef _PLAYERCONTROLLER_H_
#define _PLAYERCONTROLLER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerController
{// �R���g���[���[���N���X
public:
	CPlayerController();	// �R���X�g���N�^
	~CPlayerController();	// �f�X�g���N�^

	virtual void Update(CPlayer *pPlayer) = 0;

private:
};

class CPlayerControllerMove : public CPlayerController
{// �ړ�����N���X
public:
	CPlayerControllerMove();	// �R���X�g���N�^
	~CPlayerControllerMove();	// �f�X�g���N�^

	void Update(CPlayer *pPlayer) override;

private:
};

class CPlayerControllerCamera : public CPlayerController
{// �J��������N���X
public:
	CPlayerControllerCamera();	// �R���X�g���N�^
	~CPlayerControllerCamera();	// �f�X�g���N�^

	void Update(CPlayer *pPlayer) override;

private:
};

class CPlayerControllerAttack : public CPlayerController
{// �U������N���X
public:
	CPlayerControllerAttack();	// �R���X�g���N�^
	~CPlayerControllerAttack();	// �f�X�g���N�^

	void Update(CPlayer *pPlayer) override;

private:
};

#endif