//*****************************************************
//
// {XGóÔ[stateEnemyBoss.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "stateEnemyBoss.h"
#include "enemyBoss.h"
#include "manager.h"
#include "missile.h"
#include "bullet.h"
#include "player.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "fade.h"
#include "explosionspawner.h"
#include "particle.h"
#include "camera.h"
#include "meshfield.h"
#include "blockManager.h"
#include "orbit.h"
#include "beamBlade.h"
#include "effect3D.h"
#include "beam.h"
#include "game.h"
#include "explosionAttack.h"
#include "debrisSpawner.h"
#include "sound.h"
#include "checkPointManager.h"
#include "UIManager.h"

//*****************************************************
// èè`
//*****************************************************
namespace
{
const float TIME_MISSILE = 0.5f;	// ~TC­ËÌÔ
const int NUM_MISSILE = 10;	// ~TCÌ­Ë
const float TIME_DRONE = 1.0f;	// h[­ËÌÔ
const int NUM_DRONE = 3;	// h[Ì­Ë
const int RANGE_HEIGHT_DRONE = 500;	// h[Ì³Ì
const float MOVE_DRONE = 50.0f;	// h[ÌËoÌÚ®Ê
const float TIME_MG = 0.15f;	// }VK­ËÌÔ
const int NUM_MG = 10;	// }VKÌ­Ë
const int NUM_BEAMSMALL = 2;	// ¬r[Ì­Ë
const int NUM_ANGLEMISSILE = 5;	// ¼p~TCÌ­Ë
const float SPEED_BULLET = 200.0f;	// }VKeÌ¬x
const int ACCURACY_MG = 10;	// }VKÌ¸x
const float GRAVITY = 0.4f;	// dÍ
const float SPEED_SLASH = 0.05f;	// aÌËXs[h
const float SPEED_STEP = 0.02f;	// XebvÌXs[h
const float RANGE_SLASH = 1000.0f;	// aÉÚéÜÅÌ£
const D3DXVECTOR3 POS_CENTER = { 5170.0f,-6245.0f,3791.0f };	// Xe[WÌSÀW
const float RADIUS_BLADE = 70.0f;	// u[hÌ¼a
const float SPEED_EXPAND_BLADE = 0.01f;	// u[hÌcçÞ¬x
const float DIST_CLOSE = 3000.0f;	// ß£»èÌ£
const float DIST_FAR = 7000.0f;	// £»èÌ£
const float HEIGHT_JUMP_BEAM = -100.0f;	// WvÅ¿·éÆ«Ì³
const float WIDTH_AIRBEAM = 500.0f;	// ór[Ì
const float LENGTH_AIRBEAM = 9000.0f;	// ór[Ì·³
const float SPEED_ROTATION_BEAM = 0.02f;	// ór[Ìñ]¬x
const float LIMIT_ROT_BEAM = D3DX_PI * 0.8f;	// ór[Ìñ]§ä
const float RADIUS_PLASMA = 300.0f;	// vY}Ì¼a
const float TIME_SETEXPL = 0.005f;	// ­Ýu^C}[
const int RAND_PLASMA = 700;	// vY}Ì_ÍÍ
const float LIMIT_BACKSTEP = 5000.0f;	// obNXXebvÌ§À£
const float LIMIT_BACKSTEP_LOW = 3000.0f;	// obNXXebvÌÅá£
const int NUM_EXPLATTACK = 3;	// 1t[Éoé­Ì
const float TIME_AIRBEAM = 10.0f;	// ór[ÌÔ
const float TIME_TRANS_SECOND = 3.0f;	// æñ`ÔÉÚéÔ
}

CStateBoss::CStateBoss()
{

}

CStateBoss::~CStateBoss()
{

}

//=====================================================
// s®Ið
//=====================================================
void CStateBossSelect::Init(CEnemyBoss *pBoss)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// vC[ÆÌ£ÉæÁÄs®ªÏ»
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 pos = pBoss->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - pos;

	float fDest = D3DXVec3Length(&vecDiff);

	int nRand = universal::RandRange(100, 0);

	if (fDest < DIST_CLOSE)
	{// ß£
		Close(nRand, pBoss);
#ifdef _DEBUG
		CEffect3D::Create(pBoss->GetMtxPos(0), 600.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
	}
	else if (fDest > DIST_FAR)
	{// £
		Far(nRand, pBoss);
#ifdef _DEBUG
		CEffect3D::Create(pBoss->GetMtxPos(0), 1000.0f, 60, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
#endif
	}
	else
	{// £
		Middle(nRand, pBoss);
#ifdef _DEBUG
		CEffect3D::Create(pBoss->GetMtxPos(0), 600.0f, 60, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
#endif
	}
}

//=====================================================
// æê`Ôs®Ið
//=====================================================
void CStateBossSelect1st::Close(int nRand, CEnemyBoss *pBoss)
{// ß£
	if (nRand < 50)
	{
		pBoss->ChangeState(new CStateBossAttackMachinegun);
	}
	else
	{
		pBoss->ChangeState(new CStateBossStep1st);
	}
}

void CStateBossSelect1st::Middle(int nRand, CEnemyBoss *pBoss)
{// £
	if (nRand < 50)
	{
		pBoss->ChangeState(new CStateBossAttackMachinegun);
	}
	else
	{
		pBoss->ChangeState(new CStateBossAttackMissile);
	}
}

void CStateBossSelect1st::Far(int nRand, CEnemyBoss *pBoss)
{// £
	if (nRand < 10)
	{
		pBoss->ChangeState(new CStateBossAttackBeam);
	}
	else
	{
		pBoss->ChangeState(new CStateBossAttackMissile);
	}
}

//=====================================================
// æñ`Ôs®Ið
//=====================================================
void CStateBossSelect2nd::Close(int nRand, CEnemyBoss *pBoss)
{// ß£
	if (nRand < 10)
	{
		pBoss->ChangeState(new CStateBossSlash);
	}
	else
	{// Xebvñð
		pBoss->ChangeState(new CStateBossStep);
	}
}

void CStateBossSelect2nd::Middle(int nRand, CEnemyBoss *pBoss)
{// £
	if (nRand < 40)
	{
		pBoss->ChangeState(new CStateBossBeamSmall);
	}
	else if (nRand < 60)
	{
		pBoss->ChangeState(new CStateBossJump);
	}
	else if (nRand < 80)
	{
		pBoss->ChangeState(new CStateBossStep);
	}
	else
	{
		pBoss->ChangeState(new CStateBossSlash);
	}
}

void CStateBossSelect2nd::Far(int nRand, CEnemyBoss *pBoss)
{// £
	if (nRand < 60)
	{
		pBoss->ChangeState(new CStateBossBeamSmall);
	}
	else
	{
		pBoss->ChangeState(new CStateBossSlash);
	}
}

//=====================================================
// o»óÔ
//=====================================================
void CStateBossApper::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_APPER);

	pBoss->Rotation(D3DXVECTOR3(0.0f, 1.57f, 0.0f));
}

void CStateBossApper::Move(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// [VI¹ÅÌóÔÖÚé
		pBoss->ChangeState(new CStateBossSelect1st);
	}
}

//=====================================================
// r[U
//=====================================================
void CStateBossAttackBeam::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_SHOT);
}

void CStateBossAttackBeam::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->AimPlayer();

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// [VI¹ÅÌóÔÖÚé
		pBoss->ChangeState(new CStateBossSelect1st);
	}
}

//=====================================================
// ~TCU
//=====================================================
void CStateBossAttackMissile::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMissile::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	if (pBoss->AttackTimer(TIME_MISSILE))
	{// ~TCÌ­Ë		
		CMissile *pMissile = CMissile::Create(pBoss->GetMtxPos(1));

		if (pMissile != nullptr)
		{
			D3DXVECTOR3 rot = pBoss->GetRotation();

			if (m_nCnt % 2 == 0)
			{
				rot.x = -D3DX_PI * 0.3f;
				rot.y = D3DX_PI * 0.5f;
			}
			else
			{
				rot.x = -D3DX_PI * 0.3f;
				rot.y = -D3DX_PI * 0.3f;
			}

			pMissile->SetRotation(rot);

			D3DXVECTOR3 moveMissile =
			{
				sinf(rot.x) * sinf(rot.y + D3DX_PI) * 500.0f,
				cosf(rot.x) * 500.0f,
				sinf(rot.x) * cosf(rot.y + D3DX_PI) * 500.0f
			};

			pMissile->SetMove(moveMissile);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MISSILE)
		{// êèÁ½çÌs®Ö
			pBoss->ChangeState(new CStateBossSelect1st);
		}
	}

	// ãÞ
	pBoss->AimPlayer(0.0f, false);
}

//=====================================================
// }VK
//=====================================================
void CStateBossAttackMachinegun::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMachinegun::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	if (pBoss->AttackTimer(TIME_MG))
	{// Ëo·é
		Sound::Play(CSound::LABEL_SE_SHOT01);

		D3DXVECTOR3 posMazzle = pBoss->GetMtxPos(5);
		D3DXVECTOR3 moveBullet;
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// Î·ÌvZ
		D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);

		posPrediction.x += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.y += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.z += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);

		D3DXVECTOR3 vecDiffBullet = posPrediction - posMazzle;
		D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

		moveBullet = vecDiffBullet * SPEED_BULLET;

		// eÌ­Ë
		CBullet::Create(posMazzle, moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 1.5f);

		// AjGtFNg¶¬
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if (pAnim3D != nullptr)
		{
			pAnim3D->CreateEffect(posMazzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MG)
		{// êèÁ½çÌs®Ö
			pBoss->ChangeState(new CStateBossSelect1st);
		}
	}

	// ãÞ
	pBoss->AimPlayer(0.0f,false);
}

//=====================================================
// Xebvñð
//=====================================================
void CStateBossStep1st::Init(CEnemyBoss *pBoss)
{
	Sound::Play(CSound::LABEL_SE_DASH01);

	// ÚInðÝè
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// ÚInðÝè
	m_posDest = universal::RelativeInversPos(posPlayer, POS_CENTER, 1.0f);
	universal::LimitDistCylinder(LIMIT_BACKSTEP_LOW * 2.0f, &m_posDest, posPlayer);
	universal::LimitDistSphereInSide(LIMIT_BACKSTEP * 2.0f, &m_posDest, posPlayer);
	m_posDest.y = 0.0f;

	m_posDestMid = universal::RelativeInversPos(posPlayer, POS_CENTER, 0.0f);
	universal::LimitDistCylinder(LIMIT_BACKSTEP_LOW, &m_posDestMid, posPlayer);
	universal::LimitDistSphereInSide(LIMIT_BACKSTEP, &m_posDestMid, posPlayer);
	m_posDestMid.y = 3500.0f;

	m_bMid = false;
}

void CStateBossStep1st::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 pos = pBoss->GetPosition();
	D3DXVECTOR3 posDest = {};

	// cðo·
	pBoss->SetAfterImage(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f), 20);

	if (m_bMid)
	{// Ôn_ÖÌÚ®
		posDest = m_posDest;

		universal::MoveToDest(&pos, posDest, SPEED_STEP);

		if (universal::DistCmp(pos, posDest, RANGE_SLASH, nullptr))
		{
			pBoss->ChangeState(new CStateBossSelect1st);
		}
	}
	else
	{// ÅIÚWÖÌÚ®
		posDest = m_posDestMid;

		universal::MoveToDest(&pos, posDest, SPEED_STEP);

		if (universal::DistCmpFlat(pos, posDest, RANGE_SLASH, nullptr))
		{
			Sound::Play(CSound::LABEL_SE_DASH01);

			m_bMid = true;
		}
	}

	pBoss->SetPosition(pos);

	pBoss->AimPlayer(0.0f, false);
}

//=====================================================
// æñ`ÔÖÚs·é
//=====================================================
void CStateBossTrans::Init(CEnemyBoss *pBoss)
{
	m_fTimeTrans = 0.0f;

	CheckPointer(pBoss);

}

void CStateBossTrans::Move(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	// ­GtFNg
	D3DXVECTOR3 pos = pBoss->GetMtxPos(1);

	CParticle::Create(pos, CParticle::TYPE::TYPE_TURN_EXPLOSION);

	m_fTimeTrans += CManager::GetDeltaTime();

	CFade *pFade = CFade::GetInstance();

	int nMotion = pBoss->GetMotion();
	bool bFinish = pBoss->IsFinish();

	float fTime = TIME_TRANS_SECOND;

#ifdef _DEBUG
	fTime = 0.5f;
#endif

	if (pFade != nullptr)
	{
		CFade::FADE state = pFade->GetState();

		if (state == CFade::FADE::FADE_NONE)
		{
			if (m_fTimeTrans > fTime)
			{// tF[hÅæñ`ÔÉÚé
				Evolve(pBoss);
			}
		}

		if (state == CFade::FADE::FADE_OUT)
		{
			if (nMotion != CEnemyBoss::MOTION::MOTION_DEATH)
			{// [r[É¢­
				TransMovie(pBoss);
			}

			if (m_fTimeTrans > fTime)
			{// n`ÌÏ®
				// ubNí
				CBlockManager *pBlockManager = CBlockManager::GetInstance();

				if (pBlockManager != nullptr)
					pBlockManager->DeleteAll();

				// bVtB[hÏ`
				CMeshField *pMesh = CMeshField::GetInstance();

				if (pMesh != nullptr)
					pMesh->Load("data\\MAP\\field01.bin");

				pBoss->ChangeState(new CStateBossBeforeTrans);
			}
		}
	}
}

void CStateBossTrans::TransMovie(CEnemyBoss *pBoss)
{// [r[ÉÚs
	D3DXVECTOR3 pos = { 7000.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,D3DX_PI * 0.5f,0.0f };

	// {XÊuðÝè
	pBoss->SetPosition(pos);
	pBoss->SetRotation(rot);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// vC[ÊuðÝè
		D3DXVECTOR3 posPlayer = { 5000.0f,0.0f,0.0f };
		D3DXVECTOR3 rotPlayer = { 0.0f,0.0f,0.0f };

		pPlayer->SetPosition(posPlayer);
		pPlayer->SetRotation(rotPlayer);
	}

	// ­GtFNg
	pos.y += 400.0f;
	CExplSpawner::Create(pos, 300.0f, TIME_TRANS_SECOND * 60);
	CParticle::Create(pos, CParticle::TYPE::TYPE_TURN_EXPLOSION);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_DEATH);
}

void CStateBossTrans::Evolve(CEnemyBoss *pBoss)
{// æñ`ÔÉi»
	// å­ðN±·
	CAnimEffect3D *pAnim = CAnimEffect3D::GetInstance();

	if (pAnim != nullptr)
	{
		D3DXVECTOR3 pos = pBoss->GetMtxPos(0);

		CAnim3D *pExplosion;

		pExplosion = pAnim->CreateEffect(pos, CAnimEffect3D::TYPE_EXPLOSION);

		if (pExplosion != nullptr)
		{
			pExplosion->SetSize(2000.0f, 2000.0f);
		}
	}

	// Jðhç·
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetQuake(0.2f, 0.2f, 120);
	}

	// ¢tF[hð©¯é
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		pFade->SetFade(CScene::MODE_GAME, false);
	}
}

//=====================================================
// æñ`ÔÉÚÁ½Æ«Ì
//=====================================================
void CStateBossBeforeTrans::Init(CEnemyBoss *pBoss)
{
	pBoss->Load("data\\MOTION\\robot00.txt");

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_PRE_SLASH);

	pBoss->EnableTrans(true);
}

void CStateBossBeforeTrans::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 move = pBoss->GetMove();

	move.y -= GRAVITY;

	pBoss->SetMove(move);

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// s®ÉJÚ
		pBoss->ChangeState(new CStateBossSelect2nd);

		// ½è»èÄ¶¬
		pBoss->CreateCollision(Boss::RADIUS_COLLISION);

		// CtZbg
		pBoss->SetLife(Boss::INITIAL_LIFE);
	}
}

//=====================================================
// a[V
//=====================================================
CStateBossSlash::~CStateBossSlash()
{
	if (m_pBlade != nullptr)
	{
		m_pBlade->Uninit();
		m_pBlade = nullptr;
	}

	if (m_pOrbit != nullptr)
	{
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}
}

void CStateBossSlash::Init(CEnemyBoss *pBoss)
{
	m_pOrbit = nullptr;
	m_pBlade = nullptr;

	m_pBlade = CBeamBlade::Create();

	if (m_pBlade != nullptr)
	{
		m_pBlade->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_pBlade->SetRadius(0.0f);
	}

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_PRE_SLASH);
}

void CStateBossSlash::Move(CEnemyBoss *pBoss)
{
	int nMotion = pBoss->GetMotion();
	bool bFinish = pBoss->IsFinish();

	if (m_pBlade != nullptr)
	{// aÌXV
		// ÊuEü«ÌÇ]
		D3DXMATRIX mtxArm = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();
		D3DXMATRIX mtx;

		D3DXVECTOR3 vecBlade = universal::VecToOffset(mtxArm, D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		D3DXVECTOR3 rotBlade = universal::VecToRot(vecBlade);
		D3DXVECTOR3 posBlade =
		{
			mtxArm._41,
			mtxArm._42,
			mtxArm._43
		};

		rotBlade.x *= -1;

		m_pBlade->SetPosition(posBlade);
		m_pBlade->SetRotation(rotBlade);

		CParticle::Create(posBlade, CParticle::TYPE_BLADE_PARTICLE, rotBlade);

		// å«³ÌXV
		float fRadius = m_pBlade->GetRadius();

		fRadius += (RADIUS_BLADE - fRadius) * SPEED_EXPAND_BLADE;

		m_pBlade->SetRadius(fRadius);
	}

	if (nMotion == CEnemyBoss::MOTION::MOTION_PRE_SLASH)
	{
		pBoss->AimPlayer(0.0f, false, 0.1f);

		if (bFinish)
		{
			// ÚInðÝè
			CPlayer *pPlayer = CPlayer::GetInstance();

			if (pPlayer == nullptr)
				return;

			// vC[ðÇö
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXVECTOR3 pos = pBoss->GetPosition();

			universal::MoveToDest(&pos, posPlayer, SPEED_SLASH);

			pBoss->SetPosition(pos);

			if (universal::DistCmp(pos, posPlayer, RANGE_SLASH, nullptr))
			{
				Sound::Play(CSound::LABEL_SE_SHOT03);

				pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_SLASH);

				if (m_pOrbit == nullptr)
				{// OÕÌ¶¬
					D3DXMATRIX mtx = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();

					D3DXVECTOR3 offset = { 0.0f,-1000.0f,0.0f };

					m_pOrbit = COrbit::Create(mtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), offset, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20);
				}
			}
		}

		// r[p[eBNÌ­¶
		pBoss->BeamBlade();
	}
	else if (nMotion == CEnemyBoss::MOTION::MOTION_SLASH)
	{
		if (m_pOrbit != nullptr)
		{// OÕÌXV
			D3DXMATRIX mtx = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();

			m_pOrbit->SetPositionOffset(mtx, 0);
		}

		if (bFinish)
		{// aÌI¹
			if (m_pOrbit != nullptr)
			{
				m_pOrbit->SetEnd(true);
				m_pOrbit = nullptr;
			}

			if (m_pBlade != nullptr)
			{
				m_pBlade->Uninit();
				m_pBlade = nullptr;
			}

			pBoss->ChangeState(new CStateBossSelect2nd);
		}
	}
}

//=====================================================
// ¬r[U
//=====================================================
void CStateBossBeamSmall::Init(CEnemyBoss *pBoss)
{
	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_BEAMSMALL);

	m_nCnt = 0;
}

void CStateBossBeamSmall::Attack(CEnemyBoss *pBoss)
{
	pBoss->AimPlayer(0.0f, false, 0.07f);

	bool bShot = pBoss->AttackTimer(1.0f);

	if (bShot)
	{
		Sound::Play(CSound::LABEL_SE_SHOT03);

		// r[Ì¶¬
		CBeam *pBeam = CBeam::Create();

		D3DXMATRIX mtxParent = *pBoss->GetParts(CEnemyBoss::IDX_HAND_R)->pParts->GetMatrix();
		D3DXMATRIX mtxBeam;

		universal::SetOffSet(&mtxBeam, mtxParent, D3DXVECTOR3(0.0f, -1.0f, 1.0f));

		if (pBeam != nullptr)
		{
			D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };
			D3DXVECTOR3 posMazzle = { mtxBeam._41,mtxBeam._42 ,mtxBeam._43 };
			D3DXVECTOR3 vecMazzle = posMazzle - pos;

			D3DXVECTOR3 rot = pBoss->GetRotation();

			pBeam->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			pBeam->SetMtx(mtxBeam);
			pBeam->SetRotation(rot);
			pBeam->SetPosition(posMazzle);
			pBeam->SetAnimSize(200.0f, 10000.0f);
			pBeam->SetShrink(2.5f);
			pBeam->SetExpand(160.0f);
			pBeam->SetExtend(100.0f);

			CAnimEffect3D *pAnimEffect = CAnimEffect3D::GetInstance();

			if (pAnimEffect != nullptr)
			{
				CAnim3D *pAnim = pAnimEffect->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_BEAMFLASH);

				if (pAnim != nullptr)
				{
					D3DXVECTOR3 pos = pBoss->GetMtxPos(5);

					pAnim->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					pAnim->SetVtx();
				}
			}
		}

		for (int i = 0; i < NUM_ANGLEMISSILE; i++)
		{
			// ~TCÌU
			CMissile *pMissile = CMissile::Create(pBoss->GetMtxPos(1), CMissile::TYPE::TYPE_RIGHTANGLE);

			if (pMissile != nullptr)
			{
				D3DXVECTOR3 rot = pBoss->GetRotation();

				if (m_nCnt % 2 == 0)
				{
					rot.x = -D3DX_PI * 0.3f;
					rot.y = D3DX_PI * 0.3f;
				}
				else
				{
					rot.x = -D3DX_PI * 0.3f;
					rot.y = -D3DX_PI * 0.3f;
				}

				pMissile->SetRotation(rot);

				D3DXVECTOR3 moveMissile =
				{
					sinf(rot.x) * sinf(rot.y + D3DX_PI) * 6000.0f,
					cosf(rot.x) * 6000.0f,
					sinf(rot.x) * cosf(rot.y + D3DX_PI) * 6000.0f
				};

				pMissile->SetChaseSpeed(10.0f);
			}
		}

		m_nCnt++;

		if (m_nCnt > NUM_BEAMSMALL)
		{
			pBoss->ChangeState(new CStateBossSelect2nd);
		}
	}
}

//=====================================================
// Xebvñð
//=====================================================
void CStateBossStep::Init(CEnemyBoss *pBoss)
{
	Sound::Play(CSound::LABEL_SE_DASH01);

	// ÚInðÝè
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// ÚInðÝè
	m_posDest = universal::RelativeInversPos(posPlayer,POS_CENTER,1.0f);
	universal::LimitDistCylinder(LIMIT_BACKSTEP_LOW, &m_posDest, posPlayer);
	universal::LimitDistSphereInSide(LIMIT_BACKSTEP, &m_posDest, posPlayer);

	m_posDestMid = universal::RelativeInversPos(posPlayer, POS_CENTER, 0.0f);
	universal::LimitDistCylinder(LIMIT_BACKSTEP_LOW * 0.5f, &m_posDestMid, posPlayer);
	universal::LimitDistSphereInSide(LIMIT_BACKSTEP * 0.5f, &m_posDestMid, posPlayer);

	m_bMid = false;
}

void CStateBossStep::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 pos = pBoss->GetPosition();
	D3DXVECTOR3 posDest = {};

	// cðo·
	pBoss->SetAfterImage(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f), 20);

	if (m_bMid)
	{// Ôn_ÖÌÚ®
		posDest = m_posDest;

		universal::MoveToDest(&pos, posDest, SPEED_STEP);

		if (universal::DistCmpFlat(pos, posDest, RANGE_SLASH, nullptr))
		{
			pBoss->ChangeState(new CStateBossSelect2nd);
		}
	}
	else
	{// ÅIÚWÖÌÚ®
		posDest = m_posDestMid;

		universal::MoveToDest(&pos, posDest, SPEED_STEP);

		if (universal::DistCmpFlat(pos, posDest, RANGE_SLASH, nullptr))
		{
			Sound::Play(CSound::LABEL_SE_DASH01);

			m_bMid = true;
		}
	}

	pBoss->SetPosition(pos);

	pBoss->AimPlayerFlat(0.0f, false, 0.05f);
}

//=====================================================
// òÑãªè
//=====================================================
void CStateBossJump::Init(CEnemyBoss *pBoss)
{
	Sound::Play(CSound::LABEL_SE_DASH01);

	// ÚInðÝè
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// ÚInðÝè
	m_posDest = universal::RelativeInversPos(posPlayer, POS_CENTER, 1.0f);

	m_posDest.y = HEIGHT_JUMP_BEAM;
}

void CStateBossJump::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 pos = pBoss->GetPosition();

	universal::MoveToDest(&pos, m_posDest, SPEED_STEP);

	pBoss->SetPosition(pos);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// ÚWü«Ìæ¾
		D3DXVECTOR3 pos = pBoss->GetMtxPos(15);

		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPridiction;

		D3DXVECTOR3 vecDiff = posPlayer - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);
		rotDest.y -= D3DX_PI;

		// ü«Ìâ³
		D3DXVECTOR3 rot = pBoss->GetRotation();

		universal::FactingRot(&rot.x, 0.0f, 0.15f);
		universal::FactingRot(&rot.y, rotDest.y, 0.15f);

		pBoss->SetRotation(rot);
	}

	if (universal::DistCmp(pos, m_posDest, RANGE_SLASH, nullptr))
	{// ór[ÉÚs
		pBoss->ChangeState(new CStateBossBeamAir);
	}
}

//=====================================================
// ór[U
//=====================================================
CStateBossBeamAir::~CStateBossBeamAir()
{
	if (m_pAnim != nullptr)
	{
		m_pAnim->Uninit();
		m_pAnim = nullptr;
	}
}

void CStateBossBeamAir::Init(CEnemyBoss *pBoss)
{
	m_fTimer = 0.0f;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_PRE_AIRBEAM);

	Sound::Play(CSound::LABEL_SE_SHOT04);

	if (m_pAnim == nullptr)
	{// Aj[VGtFNgÌ¶¬
		CAnimEffect3D *pAnimEffect = CAnimEffect3D::GetInstance();

		if (pAnimEffect != nullptr)
		{
			m_pAnim = pAnimEffect->CreateEffect(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAnimEffect3D::TYPE::TYPE_BOOST);

			if (m_pAnim != nullptr)
			{
				D3DXVECTOR3 pos = pBoss->GetMtxPos(5);
				D3DXVECTOR3 rot = pBoss->GetRotation();
				rot.x += D3DX_PI;
				universal::LimitRot(&rot.x);

				m_pAnim->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_pAnim->EnableZtest(false);
				m_pAnim->SetAlphaTest(70);
				m_pAnim->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
				m_pAnim->SetSize(0.0f, 0.0f);
				m_pAnim->SetPosition(pos);
				m_pAnim->SetRotation(rot);
				m_pAnim->SetVtx();
			}
		}
	}

	m_rotDest = { 0.0f,0.0f,0.0f };
	m_posAim = { 0.0f,0.0f,0.0f };
}

void CStateBossBeamAir::DisideRotDest(CEnemyBoss *pBoss)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ÚWü«ÌÝè
	D3DXVECTOR3 posBoss = pBoss->GetPosition();

	D3DXVECTOR3 vecDiff = m_posAim - posBoss;

	m_rotDest.y = atan2f(vecDiff.x, vecDiff.z);
	m_rotDest.y -= D3DX_PI;

	float fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	m_rotDest.x = atan2f(fLength, -vecDiff.y);
	m_rotDest.x -= D3DX_PI * 0.5f;

	// GCÊuÌâ³
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 vecDiffAim = posPlayer - m_posAim;

	universal::VecConvertLength(&vecDiffAim, 4.0f);

	m_moveAim += vecDiffAim;

	universal::LimitSpeed(&m_moveAim, 300.0f);

	m_posAim += m_moveAim;

#ifdef _DEBUG
	CEffect3D::Create(m_posAim, 600.0f, 60, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
}

void CStateBossBeamAir::Attack(CEnemyBoss *pBoss)
{
	int nMotion = pBoss->GetMotion();
	bool bFinish = pBoss->IsFinish();

	if (nMotion == CEnemyBoss::MOTION::MOTION_RADIATION)
	{
		Radiation(pBoss);
	}
	else if(nMotion == CEnemyBoss::MOTION::MOTION_BEAMSMALL)
	{
		Rotation(pBoss);
	}
	else
	{
		// r[ðLÎ·
		D3DXVECTOR3 rot = pBoss->GetRotation();

		float fWidth = m_pAnim->GetWidth();

		fWidth += (WIDTH_AIRBEAM - fWidth) * 0.4f;

		float fHeight = m_pAnim->GetHeight();

		fHeight += (LENGTH_AIRBEAM - fHeight) * 0.1f;

		rot.x += D3DX_PI;
		universal::LimitRot(&rot.x);

		D3DXVECTOR3 pos = pBoss->GetMtxPos(5);
		pos +=
		{
			sinf(rot.x + D3DX_PI * 0.5f) * sinf(rot.y) * fHeight * 0.6f,
			cosf(rot.x + D3DX_PI * 0.5f) * fHeight * 0.9f,
			sinf(rot.x + D3DX_PI * 0.5f) * cosf(rot.y) * fHeight * 0.6f
		};

		rot = pBoss->GetRotation();

		m_pAnim->SetPosition(pos);
		m_pAnim->SetRotation(rot);
		m_pAnim->SetSize(fWidth, fHeight);
		m_pAnim->SetVtx();

		// ³Êðü©¹é
		universal::FactingRot(&rot.x, 0.0f, 0.15f);

		pBoss->SetRotation(rot);

		pBoss->AimPlayerFlat(0.0f, false);

		if (bFinish)
		{// ñ]ÉÚs
			pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_BEAMSMALL);
		}
	}
}

void CStateBossBeamAir::Rotation(CEnemyBoss *pBoss)
{// ñ]
	DisideRotDest(pBoss);

	if (m_pAnim != nullptr)
	{// r[ðLÎ·
		float fWidth = m_pAnim->GetWidth();

		fWidth += (WIDTH_AIRBEAM - fWidth) * 0.4f;

		float fHeight = m_pAnim->GetHeight();

		fHeight += (LENGTH_AIRBEAM - fHeight) * 0.4f;

		D3DXVECTOR3 rot = pBoss->GetRotation();
		rot.x += D3DX_PI;
		universal::LimitRot(&rot.x);

		D3DXVECTOR3 pos = pBoss->GetMtxPos(5);
		pos +=
		{
			sinf(rot.x + D3DX_PI * 0.5f) * sinf(rot.y) * fHeight * 0.9f,
				cosf(rot.x + D3DX_PI * 0.5f) * fHeight * 0.9f,
				sinf(rot.x + D3DX_PI * 0.5f) * cosf(rot.y) * fHeight * 0.9f
		};

		m_pAnim->SetPosition(pos);
		m_pAnim->SetRotation(rot);
		m_pAnim->SetSize(fWidth, fHeight);
		m_pAnim->SetVtx();

		// ­ÌÝu
		D3DXVECTOR3 posCollision =
		{
			pos.x + sinf(rot.x + D3DX_PI * 0.5f) * sinf(rot.y) * fHeight * 0.32f,
			pos.y + cosf(rot.x + D3DX_PI * 0.5f) * fHeight * 0.32f,
			pos.z + sinf(rot.x + D3DX_PI * 0.5f) * cosf(rot.y) * fHeight * 0.32f
		};

#ifdef _DEBUG
		//CEffect3D::Create(posCollision, 400.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

		CMeshField *pMesh = CMeshField::GetInstance();

		if (pMesh != nullptr)
		{
			D3DXVECTOR3 pos = pBoss->GetMtxPos(5);

			D3DXVECTOR3 posExplosion = posCollision;

			D3DXVECTOR3 vecDiff = posExplosion - pos;

			float fLengthMax = D3DXVec3Length(&vecDiff);

			for (int i = 0; i < NUM_EXPLATTACK; i++)
			{
				float fLength = (float)universal::RandRange((int)fLengthMax, 0);

				universal::VecConvertLength(&vecDiff, fLength);

				posExplosion = pos + vecDiff;

				posExplosion +=
				{
					(float)universal::RandRange(RAND_PLASMA, -RAND_PLASMA),
						(float)universal::RandRange(RAND_PLASMA, -RAND_PLASMA),
						(float)universal::RandRange(RAND_PLASMA, -RAND_PLASMA)
				};

				CExplAttack::Create(posExplosion, RADIUS_PLASMA);
			}
		}
	}

	// {Xðñ·
	D3DXVECTOR3 rot = pBoss->GetRotation();

	universal::FactingRot(&rot.x, m_rotDest.x, 0.4f);
	universal::FactingRot(&rot.y, m_rotDest.y, 0.4f);

	pBoss->SetRotation(rot);

	float fDeltaTime = CManager::GetDeltaTime();

	m_fTimer += fDeltaTime;

	if (m_fTimer > TIME_AIRBEAM)
	{
		if (m_pAnim != nullptr)
		{
			m_pAnim->Uninit();
			m_pAnim = nullptr;
		}

		pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_RADIATION);
	}
}

void CStateBossBeamAir::Radiation(CEnemyBoss *pBoss)
{// úM(ã)
	// ³Êðü­
	D3DXVECTOR3 rot = pBoss->GetRotation();

	universal::FactingRot(&rot.x, 0.0f, 0.15f);

	pBoss->SetRotation(rot);

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{
		pBoss->ChangeState(new CStateBossSelect2nd);
	}
}

//=====================================================
// S
//=====================================================
void CStateBossDeath::Init(CEnemyBoss *pBoss)
{
	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_DEATH);

	// å­Ì¶¬
	CAnimEffect3D *pAnimEffect = CAnimEffect3D::GetInstance();

	if (pAnimEffect != nullptr)
	{
		D3DXVECTOR3 pos = pBoss->GetMtxPos(0);

		CAnim3D *pAnim = pAnimEffect->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);

		if (pAnim != nullptr)
		{
			pAnim->SetSize(2000.0f, 1400.0f);
		}
	}

	// UIðñ\¦É·é
	CUIManager *pUIManager = CUIManager::GetInstance();

	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(false);
	}
}

void CStateBossDeath::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 posParticle = pBoss->GetMtxPos(CEnemyBoss::IDX_BODY);

	CParticle::Create(posParticle, CParticle::TYPE_SMOKE_DEATH);

	D3DXVECTOR3 move = pBoss->GetMove();

	move.y -= GRAVITY * 2.0f;

	pBoss->SetMove(move);

	bool bFinish = pBoss->IsFinish();
	int nMotion = pBoss->GetMotion();

	if (nMotion == CEnemyBoss::MOTION::MOTION_LAST_SHOOTING)
	{
		pBoss->AimPlayerFlat();

		CParticle::Create(posParticle, CParticle::TYPE_TURN_EXPLOSION);

		if (bFinish)
		{// å­µÄÊ
			Sound::Play(CSound::LABEL_SE_EXPLOSION01);

			// GtFNg­¶
			CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

			if (pAnimManager != nullptr)
			{
				CAnim3D *pAnim = pAnimManager->CreateEffect(posParticle, CAnimEffect3D::TYPE_EXPLOSION);

				if (pAnim != nullptr)
				{
					pAnim->SetSize(5000.0f, 4000.0f);
				}
			}

			// jÐ¶¬
			CDebrisSpawner::Create(posParticle, CDebrisSpawner::TYPE::TYPE_DEATH);

			// p[eBN
			CParticle::Create(posParticle, CParticle::TYPE_EXPLOSION_SMOKE_BIG);
			CParticle::Create(posParticle, CParticle::TYPE_EXPLOSION_FIRE_BIG);

			// Jhê
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera != nullptr)
				pCamera->SetQuake(2.0f, 2.0f, 60);

			pBoss->ChangeState(new CStateBossAfterDeath);

		}
	}
	else
	{
		if (bFinish)
		{
			pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_LAST_SHOOTING);
		}
	}
}

//=====================================================
// S
//=====================================================
void CStateBossAfterDeath::Init(CEnemyBoss *pBoss)
{
	CCheckPointManager *pCheck = CCheckPointManager::GetInstance();

	if (pCheck != nullptr)
	{
		pCheck->SetProgress(0);
	}

	CGame::SetState(CGame::STATE::STATE_END);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_AFTER_DEATH);

	// UIðñ\¦É·é
	CUIManager *pUIManager = CUIManager::GetInstance();

	if (pUIManager != nullptr)
	{
		pUIManager->EnableDisp(true);
	}
}

void CStateBossAfterDeath::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 posParticle = pBoss->GetMtxPos(CEnemyBoss::IDX_BODY);

	CParticle::Create(posParticle, CParticle::TYPE_SMOKE_DEATH);
}