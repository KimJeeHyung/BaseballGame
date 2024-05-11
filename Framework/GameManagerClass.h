#pragma once
class GameManagerClass
{
public:
	GameManagerClass();
	GameManagerClass(const GameManagerClass&);
	~GameManagerClass();

	bool Initialize();
	void Shutdown();

public:
	// 아웃카운트 함수들
	void AddOutCount();
	void ResetCount();
	int GetOutCount();
	bool IsThreeOut();

	// 타격 함수들
	bool IsTiming(float);
	void SetHit(bool);
	bool IsHit();

private:
	float m_Min = 0;
	float m_Max = 0;
	int m_OutCount = 0;
	bool m_Hit = false;
};

