//*****************************************************
//
// �ʏ�G�̏���[enemyNormal.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYDRONE_H_
#define _ENEMYDRONE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyDrone : public CEnemy
{
public:
	CEnemyDrone();	// �R���X�g���N�^
	~CEnemyDrone();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void HitField(void) override;

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@���[�V����
		MOTION_WALK,	// �������[�V����
		MOTION_ATTACK,	// �U�����[�V����
		MOTION_DEATH,	// ���S���[�V����
		MOTION_MAX
	};

	virtual void Wait(void);
	virtual void Chase(void);
	virtual void Attack(void);
	void KeepDistance(void);

	float m_fTimeFire;	// �ˎˌ��^�C�}�[
};

#endif
