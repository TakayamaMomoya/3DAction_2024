//*****************************************************
//
// �G�}�l�[�W���[[enemyManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "manager.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "inputkeyboard.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_SPAWN = 5.0f;	// �G�̃X�|�[��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CEnemyManager *CEnemyManager::m_pEnemyManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyManager::CEnemyManager()
{
	m_fTimer = 0.0f;
	m_pHead = nullptr;
	m_pTail = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyManager::~CEnemyManager()
{

}

//=====================================================
// ��������
//=====================================================
CEnemyManager *CEnemyManager::Create(void)
{
	if (m_pEnemyManager == nullptr)
	{// �C���X�^���X����
		m_pEnemyManager = new CEnemyManager;

		// ����������
		m_pEnemyManager->Init();
	}

	return m_pEnemyManager;
}

//=====================================================
// �G�̐�������
//=====================================================
CEnemy *CEnemyManager::CreateEnemy(D3DXVECTOR3 pos, CEnemy::TYPE type)
{
	char *apPath[CEnemy::TYPE_MAX] = 
	{
		nullptr,
		"data\\MOTION\\motionArms00.txt",
		"data\\MOTION\\motionEnemy01.txt",
	};

	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// �C���X�^���X����
		switch (type)
		{
		case CEnemy::TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			break;
		default:
			break;
		}

		if (pEnemy != nullptr)
		{
			// �ʒu�ݒ�
			pEnemy->SetPosition(pos);

			// ���[�V�����Ǎ�
			pEnemy->Load(apPath[type]);

			// ����������
			pEnemy->Init();
		}
	}

	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyManager::Init(void)
{
	// �Ǎ�����
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CEnemyManager::Load(void)
{

}

//=====================================================
// �I������
//=====================================================
void CEnemyManager::Uninit(void)
{
	m_pEnemyManager = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyManager::Update(void)
{
#ifdef _DEBUG

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_C))
		{
			CreateEnemy(D3DXVECTOR3(0.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
		}
	}

#endif
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyManager::Draw(void)
{
#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n�G�̈ʒu�F[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
#endif
}