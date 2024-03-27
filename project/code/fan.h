//*****************************************************
//
// 2D�~�̏���[fan2D.h]
// Author:���R����
//
//*****************************************************

#ifndef _FAN_H_
#define _FAN_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CFan	: public CObject
{
public:
	CFan(int nPriority = 6);
	~CFan();	//	�f�X�g���N�^

	virtual HRESULT Init(void);	// ����������
	virtual void Uninit(void);	// �I������
	virtual void Update(void);	// �X�V����
	virtual void Draw(void);	// �`�揈��

	void SetPosition(D3DXVECTOR3 pos);	// �ݒ菈��
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// �擾����
	void SetRotation(D3DXVECTOR3 rot);// �����ݒ菈��
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// �����擾����
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetCol(D3DXCOLOR col);
	virtual void SetVtx(void) = 0;
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }
	void SetRateAngle(float fAngle) { m_fRateAngle = fAngle; }
	float GetRateAngle(void) { return m_fRateAngle; }
	float GetRadius(void) { return m_fRadius; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetAngleMax(float fAngle) { m_fAngleMax = fAngle; }
	float GetMaxAngle(void) { return m_fAngleMax; }
	int GetNumVtx(void) { return m_nNumVtx; }
	void SetNumVtx(int nValue) { m_nNumVtx = nValue; }
	LPDIRECT3DVERTEXBUFFER9 *GetVtxBuff(void) { return &m_pVtxBuff; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
	D3DXCOLOR m_col;	// �F
	float m_fRadius;	// ���a
	float m_fRateAngle;	// �p�x�̊���
	float m_fAngleMax;	// �ő�p�x
	int m_nNumVtx;	// �O���̒��_��
	int m_nIdxTexture;	// �e�N�X�`���̔ԍ�
};

#endif