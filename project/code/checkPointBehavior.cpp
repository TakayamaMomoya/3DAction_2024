//*****************************************************
//
// �`�F�b�N�|�C���g��Behavior[checkpointBehavior.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "checkPointBehavior.h"
#include "checkPointManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//=====================================================
// �R���X�g���N�^
//=====================================================
CCheckPointBehavior::CCheckPointBehavior()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CCheckPointBehavior::~CCheckPointBehavior()
{

}

//=====================================================
// �ړ��p�[�g
//=====================================================
CCheckPointMove::CCheckPointMove()
{// �R���X�g���N�^

}

CCheckPointMove::~CCheckPointMove()
{// �f�X�g���N�^

}

void CCheckPointMove::Init(CCheckPointManager *pCheckPoint)
{// ������

}

void CCheckPointMove::Uninit(CCheckPointManager *pCheckPoint)
{// �I��

}

void CCheckPointMove::Update(CCheckPointManager *pCheckPoint)
{// �X�V

}

//=====================================================
// �퓬���
//=====================================================
CCheckPointBattle::CCheckPointBattle()
{// �R���X�g���N�^
	ZeroMemory(&m_apLimit[0], sizeof(m_apLimit));
}

CCheckPointBattle::~CCheckPointBattle()
{// �f�X�g���N�^

}

void CCheckPointBattle::Init(CCheckPointManager *pCheckPoint)
{// ������

}

void CCheckPointBattle::Uninit(CCheckPointManager *pCheckPoint)
{// �I��
	Object::DeleteObject((CObject**)&m_apLimit[0], CheckPointBehavior::NUM_LIMIT);
}

void CCheckPointBattle::Update(CCheckPointManager *pCheckPoint)
{// �X�V

}