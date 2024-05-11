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
	// �ƿ�ī��Ʈ �Լ���
	void AddOutCount();
	void ResetCount();
	int GetOutCount();
	bool IsThreeOut();

	// Ÿ�� �Լ���
	bool IsTiming(float);
	void SetHit(bool);
	bool IsHit();

private:
	float m_Min = 0;
	float m_Max = 0;
	int m_OutCount = 0;
	bool m_Hit = false;
};

