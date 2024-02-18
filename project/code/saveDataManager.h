//*****************************************************
//
//�Z�[�u�f�[�^�Ǘ�[saveDataManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _SAVEDATAMANAGER_H_
#define _SAVEDATAMANAGER_H_

#include "main.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSaveDataManager
{
public:
	CSaveDataManager();	// �R���X�g���N�^
	~CSaveDataManager();	// �f�X�g���N�^

	static CSaveDataManager *Create(void);
	static CSaveDataManager *GetInstance(void) { return m_pSaveDataManager; }
	HRESULT Init(void);
	void Uninit(void);
	int GetProgress(void) { return m_nProgress; }
	void Save(void);
	void Load(void);

private:
	int m_nProgress;	// ���݂̐i�s��
	static CSaveDataManager *m_pSaveDataManager;	// ���g�̃|�C���^
};

#endif