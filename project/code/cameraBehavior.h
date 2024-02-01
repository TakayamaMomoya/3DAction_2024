//*****************************************************
//
// �J�����r�w�C�r�A[cameraBehavior.h]
// Author:���R����
//
//*****************************************************

#ifndef _CAMERASTRATEGY_H_
#define _CAMERASTRATEGY_H_

//****************************************************
// �O���錾
//****************************************************
class CCamera;

//****************************************************
// �N���X�̒�`
//****************************************************
// ���N���X
class CCameraBehavior
{
public:
	virtual void Update(CCamera *pCamera) = 0;

private:

};

// �v���C���[�Ǐ]
class CFollowPlayer : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:
};

// �G������
class CLookEnemy : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:

};

// �G�Ƌ߂��Ƃ��̓���
class CMoveCylinder : public CCameraBehavior
{
public:
	void Update(CCamera *pCamera) override;

private:

};

#endif