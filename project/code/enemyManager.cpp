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
#include "enemyBoss.h"
#include "inputkeyboard.h"
#include "effect3D.h"
#include "player.h"
#include "UI.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_SPAWN = 5.0f;	// �G�̃X�|�[��
const float DIST_LOCKON = 5000.0f;	// ���b�N�I������
const float SIZE_CURSOR = 60.0f;	// �J�[�\���T�C�Y
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\lockon01.png";	// �J�[�\���̃e�N�X�`��
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
	m_pEnemyLockon = nullptr;
	m_bLockTarget = false;
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
		"data\\MOTION\\motionHeli.txt",
		"data\\MOTION\\motionArms00.txt",
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

	CreateEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(0.0f, 500.0f, 500.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(-1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(-1200.0f, 400.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(1500.0f, 1000.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);

	if (m_pCursor == nullptr)
	{// �J�[�\������
		m_pCursor = CUI::Create();

		if (m_pCursor != nullptr)
		{
			m_pCursor->SetSize(SIZE_CURSOR, SIZE_CURSOR);
			m_pCursor->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
			m_pCursor->SetIdxTexture(nIdx);
			m_pCursor->SetVtx();
		}
	}

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

	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

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
			CreateEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(0.0f, 500.0f, 500.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1200.0f, 400.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(1500.0f, 1000.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
		}
	}

	if(m_pEnemyLockon != nullptr)
		CEffect3D::Create(m_pEnemyLockon->GetPosition(), 30.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
}

//=====================================================
// ���b�N�I������
//=====================================================
CEnemy *CEnemyManager::Lockon(CEnemy *pEnemyExclusive)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return nullptr;
	}

	bool bLock = IsLockTarget();

	bool bInAny = false;
	CEnemy *pEnemy = GetHead();
	float fDistMax = FLT_MAX;
	D3DXVECTOR3 posCenter = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	D3DXVECTOR3 posScreen = posCenter;

	while (pEnemy != nullptr)
	{
		CEnemy::STATE state = pEnemy->GetState();

		CEnemy *pEnemyNext = pEnemy->GetNext();

		pEnemy->EnableLock(false);

		if (state != CEnemy::STATE::STATE_DEATH && pEnemyExclusive != pEnemy)
		{
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(0);
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXMATRIX mtx = *pEnemy->GetMatrix();

			D3DXVECTOR3 vecDiffPlayer = pos - posPlayer;
			float fDistPlayer = D3DXVec3Length(&vecDiffPlayer);
			D3DXVECTOR3 posScreenTemp;

			// ���b�N�I������G�̌���
			if (universal::IsInScreen(pos, mtx, &posScreenTemp))
			{
				// ��������
				if (DIST_LOCKON > fDistPlayer)
				{
					bInAny = true;

					pEnemy->EnableLock(true);
					pEnemy->SetPositionCursor(posScreenTemp);

					if (bLock == false || (m_pEnemyLockon == nullptr && bLock == true))
					{
						D3DXVECTOR3 vecDiff = posScreenTemp - posCenter;

						// ��ʒ��S����̋������v�Z
						float fDist = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

						if (fDist < fDistMax)
						{
							posScreen = posScreenTemp;

							m_pEnemyLockon = pEnemy;

							fDistMax = fDist;
						}
					}
				}
				else if (m_pEnemyLockon == pEnemy)
				{
					if (SwitchTarget(1, 1) == nullptr &&
						SwitchTarget(-1, -1) == nullptr)
					{
						m_pEnemyLockon = nullptr;
					}
				}
			}
		}

		pEnemy = pEnemyNext;
	}

	D3DXVECTOR3 posDestCursor = posCenter;

	if (bInAny == false)
	{
		if (bLock == false)
		{
			m_pEnemyLockon = nullptr;
		}
	}
	else
	{
		posDestCursor = posScreen;
	}

	if (m_pCursor != nullptr)
	{// �J�[�\���̈ʒu�␳
		D3DXVECTOR3 posCursor = m_pCursor->GetPosition();
		D3DXVECTOR3 vecDiffCursor = posDestCursor - posCursor;

		posCursor += vecDiffCursor * 0.1f;

		m_pCursor->SetPosition(posCursor);
		m_pCursor->SetVtx();
	}

	return m_pEnemyLockon;
}

//=====================================================
// �^�[�Q�b�g�̐؂�ւ�
//=====================================================
CEnemy *CEnemyManager::SwitchTarget(int nAxisX, int nAxisY)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (m_pEnemyLockon == nullptr)
	{
		return nullptr;
	}

	CEnemy *pEnemy = GetHead();
	CEnemy *pEnemyLock = nullptr;
	float fLengthDiff = FLT_MAX;

	// ���݃��b�N���Ă���G�̃X�N���[�����W
	D3DXMATRIX mtx = *m_pEnemyLockon->GetMatrix();
	D3DXVECTOR3 posScreenLockEnemy;
	D3DXVECTOR3 posLockEnemy = m_pEnemyLockon->GetMtxPos(0);
	universal::IsInScreen(posLockEnemy, mtx, &posScreenLockEnemy);

	while (pEnemy != nullptr)
	{
		CEnemy::STATE state = pEnemy->GetState();

		CEnemy *pEnemyNext = pEnemy->GetNext();

		if (state != CEnemy::STATE::STATE_DEATH && m_pEnemyLockon != pEnemy)
		{
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(0);

			D3DXVECTOR3 vecDiff = pos - pPlayer->GetPosition();
			float fDistLockon = D3DXVec3Length(&vecDiff);

			D3DXVECTOR3 posScreenTemp;

			// ���b�N�I������G�̌���
			if (universal::IsInScreen(pos, mtx, &posScreenTemp))
			{
				// ��������
				if (DIST_LOCKON > fDistLockon)
				{
					bool bOk = false;

					// �����b�N���Ă�G�Ƃ̈ʒu���ׂ�
					if (nAxisX > 0)
					{
						if (posScreenTemp.x > posScreenLockEnemy.x)
						{// ���̂��E�ɂ�����
							bOk = true;
						}
					}
					if (nAxisX < 0)
					{
						if (posScreenTemp.x < posScreenLockEnemy.x)
						{// ���̂�荶�ɂ�����
							bOk = true;
						}
					}
					if (nAxisY < 0)
					{
						if (posScreenTemp.y > posScreenLockEnemy.y)
						{// ���̂�艺�ɂ�����
							bOk = true;
						}
					}
					if (nAxisY > 0)
					{
						if (posScreenTemp.y < posScreenLockEnemy.y)
						{// ���̂���ɂ�����
							bOk = true;
						}
					}

					if (bOk)
					{
						// �����������擾
						D3DXVECTOR3 vecDIffScreen = posScreenTemp - posScreenLockEnemy;

						float fLengthScreen = D3DXVec3Length(&vecDIffScreen);

						if (fLengthScreen < fLengthDiff)
						{// �ő勗�����Z�������玟�̃��b�N�I���ɂ���
							pEnemyLock = pEnemy;
							fLengthDiff = fLengthScreen;
						}
					}
				}
			}
		}

		pEnemy = pEnemyNext;
	}

	if (pEnemyLock != nullptr)
	{
		m_pEnemyLockon = pEnemyLock;
	}

	return pEnemyLock;
}

//=====================================================
// �^�[�Q�b�g�̃��b�N
//=====================================================
void CEnemyManager::EnableLockTarget(bool bLock)
{
	m_bLockTarget = bLock;
}

//=====================================================
// ���S�����G�����b�N�I�����Ă���̂Ɠ������ǂ���
//=====================================================
void CEnemyManager::CheckDeathLockon(CEnemy *pEnemy)
{
	if (pEnemy == m_pEnemyLockon)
	{
		m_pEnemyLockon = nullptr;

		if (SwitchTarget(1, 1) == nullptr &&
			SwitchTarget(-1, -1) == nullptr)
		{
			m_pEnemyLockon = nullptr;
		}
	}
}

//=====================================================
// �S�G���폜����
//=====================================================
void CEnemyManager::DeleteAll(void)
{
	CEnemy *pEnemy = GetHead();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemyNext = pEnemy->GetNext();

		pEnemy->Uninit();

		pEnemy = pEnemyNext;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyManager::Draw(void)
{
#ifdef _DEBUG

	if (m_pEnemyLockon != nullptr)
	{
		CDebugProc::GetInstance()->Print("\n���b�N�I�����Ă�G�����");
	}
#endif
}