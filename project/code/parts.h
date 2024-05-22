//*****************************************************
//
// �p�[�c�̏���[parts.h]
// Author:���R����
//
//*****************************************************

#ifndef _PARTS_H_
#define _PARTS_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_TEX	(5)	// �e�N�X�`���̐�

//*****************************************************
// �N���X��`
//*****************************************************
class CParts : public CObjectX
{
public:
	CParts();	// �R���X�g���N�^
	~CParts();	// �f�X�g���N�^

	static CParts *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosOrg(void) { return m_posOrg; }
	void SetPosOrg(D3DXVECTOR3 pos) { m_posOrg = pos; }

private:
	D3DXVECTOR3 m_posOrg;								//�ŏ��̈ʒu
};

#endif