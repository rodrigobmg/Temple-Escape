#ifndef __Slime_H__
#define __Slime_H__

#include "Enemy.h"
#include "p2Animation.h"
#include "p2DynArray.h"

#define TILE_RADAR 48

class Slime : public Enemy
{
private:
	fPoint original_pos;
	int slime_IA; 
	bool slime_going_right, moving, player_in_radar;
	fPoint movementSpeed;
	iPoint movementGoal, playerGoal;

	iPoint tile_radar[TILE_RADAR];

public:

	Slime(int x, int y);
	void OnCollision(Collider* collider);
	void Move(float dt);
	void SetRadar();
	bool CheckForPlayer();
	uint getLives();

	void SetMovementWithPath(const p2DynArray<iPoint>* path, float dt, iPoint position);
};

#endif