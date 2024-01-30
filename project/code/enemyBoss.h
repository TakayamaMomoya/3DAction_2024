//*****************************************************
//
// �{�X�G�̏���[enemyboss.h]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYBOSS_H_
#define _ENEMYBOSS_H_

#include "enemy.h"

class CStateBoss;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyBoss : public CEnemy
{
public:
	enum MOTION
	{// ���[�V����
		MOTION_APPER = 0,	// �o�����[�V����
		MOTION_MISSILE,	// �~�T�C�����[�V����
		MOTION_DASH,	// �ːi���[�V����
		MOTION_SHOT_UNDER,	// �ˌ����[�V����
		MOTION_SHOT_UPPER,	// ���]�ˌ����[�V����
		MOTION_DEATH,	// ���S���[�V����
		MOTION_MAX
	};

	CEnemyBoss();	// �R���X�g���N�^
	~CEnemyBoss();	// �f�X�g���N�^

	static CEnemyBoss *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemyBoss *GetInstance(void) { return m_pEnemyBoss; }
	void Hit(float fDamage);
	void ChangeState(CStateBoss *pNext);
	void Event(EVENT_INFO *pEventInfo);

private:
	enum IDXPARTS
	{// �p�[�c�̔ԍ�
		IDX_WAIST = 0,	// ��
		IDX_BODY,	// ��
		IDX_HEAD,	// ��
		IDX_SHOULDER_L,	// ����
		IDX_ARM_L,	// ���r
		IDX_HAND_L,	// ����
		IDX_SHOULDER_R,	// �E��
		IDX_ARM_R,	// �E��
		IDX_HAND_R,	// �E��
		IDX_LEG_L,	// ����
		IDX_FOOT_L,	// ����
		IDX_TOE_L,	// ������
		IDX_LEG_R,	// �E��
		IDX_FOOT_R,	// �E��
		IDX_TOE_R,	// �E����
		IDX_WEAPON,	// ����
		IDX_MAX
	};
	struct Sinfo
	{// ���g�̏��
		CStateBoss *pState;	// ���
		D3DXVECTOR3 posDest;	// �ڕW�ʒu
		int nCntAttack;	// �U���J�E���^�[
		int nNumAttack;	// �U��������
		int nCntState;	// ��ԑJ�ڃJ�E���^�[
	};

	void ManageCollision(void);
	void FollowCollision(void);
	bool FollowDest(void);

	static CEnemyBoss *m_pEnemyBoss;	// ���g�̃|�C���^
	Sinfo m_info;	// ���g�̏��
};

#endif
