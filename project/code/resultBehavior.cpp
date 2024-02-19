//*****************************************************
//
// ���U���g��Behavior[resultBehavior.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultBehavior.h"
#include "number.h"
#include "result.h"
#include "saveDataManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_PLACE = 6;

const D3DXVECTOR3 POS_NUMBER[CResultPlayerScore::TYPE_MAX] =
{// �����\���X�R�A�̐��l�̈ʒu
	{ SCRN_MID.x, SCRN_MID.y - 170.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 30.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 120.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 280.0f, 0.0f },
};

const D3DXVECTOR2 SIZE_NUMBER = { 25.0f,50.0f };	// �����̃T�C�Y

const int COST_REPAIR = 50;	// ������1%�ɂ�����C����p
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResultBehavior::CResultBehavior()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CResultBehavior::~CResultBehavior()
{

}

//=====================================================
// �v���C���[�X�R�A�\��
//=====================================================
CResultPlayerScore::CResultPlayerScore()
{// �R���X�g���N�^
	ZeroMemory(&m_apNumber[0], sizeof(m_apNumber));
}

CResultPlayerScore::~CResultPlayerScore()
{// �f�X�g���N�^
	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{
		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->Uninit();
			m_apNumber[i] = nullptr;
		}
	}
}

void CResultPlayerScore::Init(CResult *pResult)
{// ������
	int nBaseReward = Result::BASE_REWARD;
	int nRepair = 0;
	int nAddReward = 0;

	// �Z�[�u�f�[�^�̎擾
	CSaveDataManager *pSave = CSaveDataManager::GetInstance();

	if (pSave == nullptr)
		assert(("�Z�[�u�f�[�^�̎擾���ł��܂���", false));

	CSaveDataManager::SInfo info = *pSave->GetInfo();

	nAddReward = info.nAddReward;

	// �C����p�v�Z
	float fRate = info.fLife / info.fInitialLife;

	fRate = (1.0f - fRate) * 100.0f;

	nRepair = COST_REPAIR * (int)fRate;

	// ���z
	int nResult = nBaseReward + nAddReward - nRepair;

	int aValue[TYPE_NUMBER::TYPE_MAX] =
	{// �l�̕ۑ��p
		nBaseReward,nAddReward,nRepair,nResult
	};

	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{// �����̐���
		m_apNumber[i] = CNumber::Create(NUM_PLACE, 0);

		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->SetSizeAll(SIZE_NUMBER.x, SIZE_NUMBER.y);
			m_apNumber[i]->SetPosition(POS_NUMBER[i]);
			m_apNumber[i]->SetValue(aValue[i],NUM_PLACE);
		}
	}
}

void CResultPlayerScore::Update(CResult *pResult)
{// �X�V

}