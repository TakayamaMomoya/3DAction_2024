//*****************************************************
//
// �~�T�C���̏���[enemymissile.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMissile : public CEnemy
{// �v���C���[�Ɍ������G
public:
	CMissile();	// �R���X�g���N�^
	~CMissile();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMissile *Create(D3DXVECTOR3 pos);
	void Hit(float fDamage) { Death(); }
	void HitField(void) override;
	void SetChaseSpeed(float fSpeed) { m_fChaseSpeed = fSpeed; }

private:
	void ChasePlayer(void);	// �ǐՏ���
	void Tilt(void);	// �X������
	void Death(void);	// ���S����

	float m_fTimerHit;	// ���e�^�C�}�[
	int m_nDeathTimer;	// ���S�J�E���^�[
	bool m_bEndChase;	// �ǔ��I���t���O
	float m_fChaseSpeed;	// �ǔ����\
};

#endif
