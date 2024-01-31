#pragma once
#include "Enemy.h"

class Enemy;

class EnemyState
{
protected:
	Enemy* enemy_ = nullptr;

public:
	virtual void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	virtual void Update() {};
};

class EnemyStateApproah : public EnemyState {
public:
	void Update();
};

class EnemyStateLeave : public EnemyState {
public:
	void Update();
};

