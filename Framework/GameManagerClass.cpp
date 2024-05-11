#include "GameManagerClass.h"

GameManagerClass::GameManagerClass()
{
}

GameManagerClass::GameManagerClass(const GameManagerClass&)
{
}

GameManagerClass::~GameManagerClass()
{
}

bool GameManagerClass::Initialize()
{
    m_Min = -64.f;
    m_Max = -57.f;
    return true;
}

void GameManagerClass::Shutdown()
{
}

bool GameManagerClass::IsTiming(float ballZPos)
{
    // 공의 Z위치가 범위 안에 들어오면 true 리턴
    if (ballZPos < m_Max && ballZPos > m_Min)
    {
        return true;
    }

    return false;
}

void GameManagerClass::AddOutCount()
{
    m_OutCount++;
}

void GameManagerClass::ResetCount()
{
    m_OutCount = 0;
}

int GameManagerClass::GetOutCount()
{
    return m_OutCount;
}

bool GameManagerClass::IsThreeOut()
{
    if (m_OutCount == 3)
    {
        return true;
    }

    return false;
}

void GameManagerClass::SetHit(bool hit)
{
    m_Hit = hit;
}

bool GameManagerClass::IsHit()
{
    return m_Hit;
}
