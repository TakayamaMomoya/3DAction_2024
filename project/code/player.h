//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CObject3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayer : public CMotion
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_BLOW,	// ������΂���Ă�����
		STATE_DAMAGE,	// �_���[�W���
		STATE_DEATH,	// ���S���
		STATE_MAX
	};
	struct SParam
	{
		float fSpeedMove;		// �ړ����x
		float fInitialLife;	// �����̗�
	};

	CPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// �ҋ@
		MOTION_WALK_FRONT,	// �O��
		MOTION_SHOT,	// �ˌ�
		MOTION_JUMP,	// �W�����v
		MOTION_AIR,	// �؋�
		MOTION_ASSAULT,	// �ːi���
		MOTION_MELEE,	// �ߐڍU��
		MOTION_MELEE2,	// �Q��ڂ̋ߐڍU��
		MOTION_STOP,	// �}��~���[�V����
		MOTION_MAX
	};
	struct SFragMotion
	{
		bool bMove;	// �ړ�
		bool bJump;	// �W�����v
		bool bShot;	// �ˌ�
		bool bMelee;	// �ߐڍU��
		bool bAddAttack;	// �ǉ��U��
		bool bAir;	// �؋�
		bool bStop;	// �}��~
	};
	struct SInfo
	{
		float fLife;	// �̗�
		STATE state;	// ���
		CCollisionSphere *pCollisionSphere;	// ���̓����蔻��
		CCollisionSphere *pClsnAttack;	// �U���̓����蔻��
		CCollisionCube *pCollisionCube;	// �����̂̓����蔻��
		bool bLand;	// ���n���Ă��邩�ǂ���
	};

	void Input(void);
	void InputMove(void);
	void InputCamera(void);
	void InputAttack(void);
	void Rotation(void);
	void ManageCollision(void);
	void ManageState(void);
	void ManageMotion(void);
	void AddMoveForward(float fSpeed);
	void Event(EVENT_INFO *pEventInfo);
	void Shot(D3DXVECTOR3 posMazzle);
	void ManageAttack(D3DXVECTOR3 pos,float fRadius);
	void Debug(void);

	SInfo m_info;	// ���g�̏��
	SParam m_param;	// �p�����[�^�[���
	SFragMotion m_fragMotion;	// ���[�V�����̃t���O

	static CPlayer *m_pPlayer;	// ���g�̃|�C���^
};

#endif