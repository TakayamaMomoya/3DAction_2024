//*****************************************************
//
// �r�[���u���[�h�̏���[beamBlade.h]
// Author:���R����
//
//*****************************************************

#ifndef _BEAMBLADE_H_
#define _BEAMBLADE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMeshCylinder;
class CObject3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBeamBlade : public CObject
{
public:
	CBeamBlade(int nPriority = 3);	// �R���X�g���N�^
	~CBeamBlade();	// �f�X�g���N�^

	static CBeamBlade *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	struct SInfo
	{
		CMeshCylinder *pCylinder;
		CObject3D *pBlade;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	SInfo m_info;
};

#endif