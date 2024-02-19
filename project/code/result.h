//*****************************************************
//
// ���U���g�̏���[result.h]
// Author:���R����
//
//*****************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "scene.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace Result
{
const int BASE_REWARD = 100000;	// ��{��V
}

//*****************************************************
// �O���錾
//*****************************************************
class CMeshCylinder;
class CResultBehavior;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CResult : public CScene
{
public:
	
	CResult();	// �R���X�g���N�^
	~CResult();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeBehavior(CResultBehavior *pBehavior);

private:
	CResultBehavior *m_pBehavior;
	CMeshCylinder *m_pCylinder;	// �w�i�̃V�����_�[
};

#endif