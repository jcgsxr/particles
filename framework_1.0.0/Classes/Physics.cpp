/*
 *  Physics.cpp
 *  framework
 *
 *  Created by Johnny Chan on 12/5/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */


#include "physics_types.h"
#include "Physics.h"
#include "SystemDefines.h"
#include "Engine.h"
#include <cmath>
#include "MathUtil.h"
#include "Graphics.h"
#include "types.h"
#include "Vector2.h"


void CPhysics::initPhysicsObject(physicsObject *object, float x, float y)
{
	object->pos.x = x;
	object->pos.y = y;
	
	initPhysicsObject(object);
}

void CPhysics::initPhysicsObject(physicsObject *object)
{
	object->type = ePhysicsTypeNone;
	object->active = TRUE;
	object->acc = Vector3(0.0f, 0.0f, 0.0f);
	object->vel = Vector3(0.0f, 0.0f, 0.0f);
	object->vel_cap = Vector3(0.0f, 0.0f, 0.0f);
	object->res_vel = Vector3(0.0f, 0.0f, 0.0f);
	object->weight = 0.0;
	object->bounciness = 0.0;
	object->elasticity = 0.0;
	object->hardness = 0.0;
	object->resistance = 1.0;
	
	object->vel_release_threshold = Vector3(0.0f, 0.0f, 0.0f);
	
	object->x_vel_zeroed_out = FALSE;
	object->y_vel_zeroed_out = FALSE;
	
	object->will_cause_y_zero_out = FALSE;
	object->will_cause_x_zero_out = FALSE;
	
	object->will_cause_y_zero_out_chain = FALSE;
	object->will_cause_x_zero_out_chain = FALSE;
	
	object->movement_state = PHYSICS_MOVEMENT_FORWARD;
	object->physics_counter = 0;
}

void CPhysics::initCircleObject(POCircle *object, float radius)
{
	initPhysicsObject(object);
	object->type = ePhysicsTypeCircle;
	object->radius = radius;
}

void CPhysics::initLineObject(POLine *object, float startX, float startY, float endX, float endY)
{
	initPhysicsObject(object);
	object->type = ePhysicsTypeLine;
	object->start.x = startX;
	object->start.y = startY;
	object->end.x = endX;
	object->end.y = endY;
}


void CPhysics::initGravWellObject(POGravWell *object, float gravConst)
{
	initPhysicsObject(object);
	object->type = ePhysicsTypeGravWell;
	object->grav_const = gravConst;
	object->is_attract = TRUE;
}


void CPhysics::resetVelocity(physicsObject *object)
{	
	object->vel.x = 0.0;
	object->vel.y = 0.0;
	object->vel.z = 0.0;
	object->res_vel.x = 0.0;
	object->res_vel.y = 0.0;
	object->res_vel.z = 0.0;
}

void CPhysics::updateCollisionsAndPhysics(physicsObject **objects, int numObjects, float threshold)
{
	for (int i = 0; i < numObjects; ++i)
	{			
		for (int j = i + 1; j < numObjects; ++j)
		{		
			//printf("res_t: %f \n", res_t);
			
			if (checkObjectCollisions(objects[i], objects[j]))
			{
				// Execute collision reaction.
				if (threshold == 0.0)
				{
					runObjectCollisionReactions(objects[i], objects[j]);
				}
				else
				{
					runObjectCollisionReactionsWithThreshold(objects[i], objects[j], PHYS_AXIS_X, 4.0);
				}
				//circleCircleCollisionWithThreshold(&_obj[i], &_obj[j], PHYS_AXIS_X, 4.0);
			}
		}
	}
	
	for (int i = 0; i < numObjects; ++i)
	{	
		updatePhysics(objects[i]);
	}
}


void CPhysics::runObjectCollisionReactionsWithThreshold(physicsObject *obj1, physicsObject *obj2, ePhysicsAxis axis, float threshold)
{
	if ((obj1->type == ePhysicsTypeCircle) && (obj2->type == ePhysicsTypeCircle))
	{
		circleCircleCollisionWithThreshold((POCircle*)obj1, (POCircle*)obj2, axis, threshold);
	}
}

void CPhysics::runObjectCollisionReactions(physicsObject *obj1, physicsObject *obj2)
{
	if ((obj1->type == ePhysicsTypeCircle) && (obj2->type == ePhysicsTypeCircle))
	{
		// Run the circle-circle collision reaction.
		circleCircleCollision((POCircle*)obj1, (POCircle*)obj2);
	}
	else if ((obj1->type == ePhysicsTypeLine) && (obj2->type == ePhysicsTypeLine))
	{
		// Run the line-line collision.
		// TODO: Implement the line-line collision reaction.

	}
	else if ((obj1->type == ePhysicsTypeRect) && (obj2->type == ePhysicsTypeRect))
	{
		// Run the rect-rect collision.
		// TODO: Implement the rect-rect collision reaction.

	}
	else if ( ((obj1->type == ePhysicsTypeLine) && (obj2->type == ePhysicsTypeCircle)) ||
			 ((obj1->type == ePhysicsTypeCircle) && (obj2->type == ePhysicsTypeLine)) )
	{
		// Run the line-circle collision reaction.
		circleLineCollision((POLine*)obj1, (POCircle*)obj2);
	}
	else if  ( ((obj1->type == ePhysicsTypeRect) && (obj2->type == ePhysicsTypeCircle)) ||
			  ((obj1->type == ePhysicsTypeCircle) && (obj2->type == ePhysicsTypeRect)) )
	{
		// Run the rect-circle collision.
		// TODO: Implement the rect-circle collision reaction.

	}
	else if ( ((obj1->type == ePhysicsTypeRect) && (obj2->type == ePhysicsTypeLine)) ||
			 ((obj1->type == ePhysicsTypeLine) && (obj2->type == ePhysicsTypeRect)) )
	{
		// Run the rect-line collision.
		// TODO: Implement the rect-line collision reaction.

	}
}

BOOL CPhysics::checkObjectCollisions(const physicsObject *obj1, const physicsObject *obj2)
{
	if ((obj1->type == ePhysicsTypeCircle) && (obj2->type == ePhysicsTypeCircle))
	{
		// Return the circle-circle collision test result.
		return checkCircleCircleCollision((POCircle*)obj1, (POCircle*)obj2);
	}
	else if ((obj1->type == ePhysicsTypeLine) && (obj2->type == ePhysicsTypeLine))
	{
		// Return the line-line intersection test result.
		return checkLineLineIntersection((POLine*)obj1, (POLine*)obj2);
	}
	else if ((obj1->type == ePhysicsTypeRect) && (obj2->type == ePhysicsTypeRect))
	{
		// Return the rect-rect collision test result.
		// TODO: Implement the rect-rect collision test.
		return FALSE;
	}
	else if ( ((obj1->type == ePhysicsTypeLine) && (obj2->type == ePhysicsTypeCircle)) ||
			 ((obj1->type == ePhysicsTypeCircle) && (obj2->type == ePhysicsTypeLine)) )
	{
		// Return the line-circle collision test result.
		return checkCircleRayCollision((POLine*)obj1, (POCircle*)obj2);
	}
	else if  ( ((obj1->type == ePhysicsTypeRect) && (obj2->type == ePhysicsTypeCircle)) ||
			  ((obj1->type == ePhysicsTypeCircle) && (obj2->type == ePhysicsTypeRect)) )
	{
		// Return the rect-circle collision test result.
		// TODO: Implement the rect-circle collision test.
		return FALSE;
	}
	else if ( ((obj1->type == ePhysicsTypeRect) && (obj2->type == ePhysicsTypeLine)) ||
			 ((obj1->type == ePhysicsTypeLine) && (obj2->type == ePhysicsTypeRect)) )
	{
		// Return the line-rect collision test result.
		// TODO: Implement the line-rect collision test.
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}


void CPhysics::updatePhysics(physicsObject *object, bool enableFrameSkip)
{
	updatePhysics(object, TIME_LAST_FRAME, enableFrameSkip);
}


void CPhysics::updatePhysics(physicsObject *object, float time, bool enableFrameSkip)
{
	if (object->active == FALSE)
	{
		return;
	}

	int num_updates = 1;
	if (enableFrameSkip)
	{
		// If there is any chugging, we need to figure out how many times the update needs to be run.
		num_updates = (int)time / PHYSICS_DELAY_MS;
	}
	
	// At least one update must occur.
	if (num_updates == 0)
	{
		num_updates = 1;
	}		
	
	// Update the physics counter.
	// TODO: Figure out what should be done when an object's velocity is locked.
	if (object->movement_state == PHYSICS_MOVEMENT_FORWARD)
	{
		object->physics_counter += num_updates;
	}
	else
	{
		//object->physics_counter -= num_updates;
		// TODO: Fix issue where updates were being skipped during reverse mode. Setting updates to 1 here is a band-aid solution.
		num_updates = 1;
		object->physics_counter -= num_updates;
	}

	if (object->physics_counter < 0)
	{
		object->physics_counter = 0;
		
		//printf("object->physics_counter: %d \n", object->physics_counter);
		
		// Don't allow for physics to go back past the beginning of TIME!
		return;
	}
	
	//printf("num_updates: %d\n", num_updates);
	
	for (int i = 0; i < num_updates; ++i)
	{
		// Only update the object if the velocity has not been locked.
		if (!object->x_vel_zeroed_out)
		{
			object->prev_pos.x = object->pos.x;
			object->res_vel.x += (object->acc.x * object->movement_state);
			
			// Apply resistance if it exists
			//if (object->resistance < 1.0)
			{
				// Regain energy if we are moving backwards in time.
				if (object->movement_state == PHYSICS_MOVEMENT_REWIND)
				{
					object->res_vel.x /= object->resistance;
				}
				else
				{
					object->res_vel.x *= object->resistance;					
				}				
			}
			
			// Check if a velocity cap was reached.  Ignore check if the cap is zero.
			if (object->vel_cap.x != 0.0)
			{
				// Check if the velocity cap was reached or passed.
				if (fabs(object->res_vel.x) > object->vel_cap.x)
				{
					// Cap according to sign.
					if (object->res_vel.x < 0)
					{
						object->res_vel.x = -object->vel_cap.x;
						object->vel.x = -object->vel_cap.x;
					}
					else
					{
						object->res_vel.x = object->vel_cap.x;
						object->vel.x = object->vel_cap.x;
					}
				}
				else
				{
					// Set velocity normally if the cap wasn't reached.
					object->vel.x = object->res_vel.x;
				}
			}
			else
			{
				object->vel.x = object->res_vel.x;
			}
			
			// Check whether the velocity is low enough to just zero out.
			if (fabs(object->vel.x) < VELOCITY_EPSILON)
			{
				object->res_vel.x = 0.0;
				object->vel.x = 0.0;
			}
			
			object->pos.x += (object->vel.x * PHYSICS_DELAY * object->movement_state);
		}
		
		if (!object->y_vel_zeroed_out)
		{
			object->prev_pos.y = object->pos.y;
			object->res_vel.y += (object->acc.y * object->movement_state);
			
			// Apply resistance if it exists
			//if (object->resistance < 1.0)
			{
				// Regain energy if we are moving backwards in time.
				if (object->movement_state == PHYSICS_MOVEMENT_REWIND)
				{
					object->res_vel.y /= object->resistance;
				}
				else
				{
					object->res_vel.y *= object->resistance;
				}
			}
			
			// Check if a velocity cap was reached.  Ignore check if the cap is zero.
			if (object->vel_cap.y != 0.0)
			{
				// Check if the velocity cap was reached or passed.
				if (fabs(object->res_vel.y) > object->vel_cap.y)
				{
					// Cap according to sign.
					if (object->res_vel.y < 0)
					{
						object->res_vel.y = -object->vel_cap.y;
						object->vel.y = -object->vel_cap.y;
					}
					else
					{
						object->res_vel.y = object->vel_cap.y;
						object->vel.y = object->vel_cap.y;
					}
				}
				else
				{
					// Set velocity normally if the cap wasn't reached.
					object->vel.y = object->res_vel.y;
				}
			}
			else
			{
				object->vel.y = object->res_vel.y;
			}
			
			// Check whether the velocity is low enough to just zero out.
			if (fabs(object->vel.y) < VELOCITY_EPSILON)
			{
				object->res_vel.y = 0.0;
				object->vel.y = 0.0;
			}
			
			object->pos.y += (object->vel.y * PHYSICS_DELAY * object->movement_state);
		}
		
		if (!object->z_vel_zeroed_out)
		{
			object->prev_pos.z = object->pos.z;
			object->res_vel.z += (object->acc.z * object->movement_state);
			
			// Apply resistance if it exists
			//if (object->resistance < 1.0)
			{
				// Regain energy if we are moving backwards in time.
				if (object->movement_state == PHYSICS_MOVEMENT_REWIND)
				{
					object->res_vel.z /= object->resistance;
				}
				else
				{
					object->res_vel.z *= object->resistance;
				}
			}
			
			// Check if a velocity cap was reached.  Ignore check if the cap is zero.
			if (object->vel_cap.z != 0.0)
			{
				// Check if the velocity cap was reached or passed.
				if (fabs(object->res_vel.z) > object->vel_cap.z)
				{
					// Cap according to sign.
					if (object->res_vel.z < 0)
					{
						object->res_vel.z = -object->vel_cap.z;
						object->vel.z = -object->vel_cap.z;
					}
					else
					{
						object->res_vel.z = object->vel_cap.z;
						object->vel.z = object->vel_cap.z;
					}
				}
				else
				{
					// Set velocity normally if the cap wasn't reached.
					object->vel.z = object->res_vel.z;
				}
			}
			else
			{
				object->vel.z = object->res_vel.z;
			}
			
			// Check whether the velocity is low enough to just zero out.
			if (fabs(object->vel.z) < VELOCITY_EPSILON)
			{
				object->res_vel.z = 0.0;
				object->vel.z = 0.0;
			}
			
			object->pos.z += (object->vel.z * PHYSICS_DELAY * object->movement_state);
		}
	}
}


void CPhysics::updateGravWell(const POGravWell *gravWell, physicsObject *object, bool enableFrameSkip)
{
	updateGravWell(gravWell, object, TIME_LAST_FRAME, enableFrameSkip);
}


void CPhysics::updateGravWell(const POGravWell *gravWell, physicsObject *object, float time, bool enableFrameSkip)
{
	if (object->active == FALSE)
	{
		return;
	}
	
	int num_updates = 1;
	if (enableFrameSkip)
	{
		// If there is any chugging, we need to figure out how many times the update needs to be run.
		num_updates = (int)time / PHYSICS_DELAY_MS;
	}
	
	//  Must update at least once.
	if (num_updates < 1)
	{
		num_updates = 1;
	}
	
	// Calculate the vector from the object to the gravity well.
	float dist = distBetweenPO(*gravWell, *object);
	// For the purposes of calculaing gravity power, we must never let distance be less than 1 to prevent super large forces to occur.
	if (dist <= 1)
	{
		dist = 1;
	}
	
	// Determine gravity power. Notice that as the distance increases, gravity power decreases.
	float grav_pow = gravWell->grav_const / dist;
	
	// Normalize the vector values and multiply in the gravity power value to get the force to be applied to the object.
	float force_x = ((gravWell->pos.x - object->pos.x) / dist) * grav_pow;
	float force_y = ((gravWell->pos.y - object->pos.y) / dist) * grav_pow;
	float force_z = ((gravWell->pos.z - object->pos.z) / dist) * grav_pow;
	
	if (!gravWell->is_attract)
	{
		force_x = -force_x;
		force_y = -force_y;
		force_z = -force_z;
	}
	
	
	for (int i = 0; i < num_updates; ++i)
	{
		// Apply the force.
		applyImpulse(object, force_x, force_y, force_z);
	}
}
							  

void CPhysics::updateAcceleration(physicsObject *object)
{
	if (object->active == FALSE)
	{
		return;
	}
	
	object->res_vel.x += object->acc.x * ((float)TIME_LAST_FRAME / 1000);
	object->res_vel.y += object->acc.y * ((float)TIME_LAST_FRAME / 1000);
}


void CPhysics::circleScreenCollision(POCircle *circle)
{
	if (circle->active == FALSE)
	{
		return;
	}
	
	// Check the top of the screen.
	if ((circle->pos.y - circle->radius) < 0)
	{
		// Calculate the x position based on how much we moved y.
		circle->pos.x += (circle->res_vel.x / circle->res_vel.y) * (circle->pos.y - circle->radius);
		
		// Next, adjust the y position.
		circle->pos.y = circle->radius;
		
		// Only reverse the velocity if it is moving towards the top of the screen.
		if (circle->res_vel.y < 0)
		{
			circle->res_vel.y = -(circle->res_vel.y * circle->bounciness);
		}
		// Update vel with friction coefficient.
		circle->res_vel.y *= FRICTION_COEFF;
		
		// This doesn't really belong here but since we're trying to simulate real physics with a gravity value we will apply x friction here as well.
		circle->res_vel.x *= FRICTION_COEFF;
	}
	
	// Check bottom of the screen.
	if ((circle->pos.y + circle->radius) > SCRN_H)
	{
		// Calculate the x position based on how much we moved y.
		circle->pos.x += (circle->res_vel.x / circle->res_vel.y) * (SCRN_H - (circle->pos.y + circle->radius));
		
		// Next, adjust the y position.
		circle->pos.y = (SCRN_H - circle->radius);
		
		// Only reverse the vel if it is moving towards the bottom.
		if (circle->res_vel.y > 0)
		{
			circle->res_vel.y = -(circle->res_vel.y * circle->bounciness);
		}
		
		// Update vel with friction coefficient.
		circle->res_vel.y *= FRICTION_COEFF;

		// This doesn't really belong here but since we're trying to simulate real physics with a gravity value we will apply x friction here as well.
		circle->res_vel.x *= FRICTION_COEFF;
	}
	
	// Check left of the screen.
	if ((circle->pos.x - circle->radius) < 0.0)
	{
		// Calculate the y position based on how much we moved x.
		circle->pos.y += (circle->res_vel.y / circle->res_vel.x) * (circle->pos.x - circle->radius);
		
		// Next, adjust the x position.
		circle->pos.x = circle->radius;
			
		// Only reverse the vel if it is moving towards the left.
		if (circle->res_vel.x < 0)
		{
			circle->res_vel.x = -(circle->res_vel.x * circle->bounciness);
		}
		
		// Update vel with friction coefficient.
		circle->res_vel.x *= FRICTION_COEFF;
	}
	
	// Check right of the screen.
	if ((circle->pos.x + circle->radius) > SCRN_W)
	{
		// Calculate the y position based on how much we moved x.
		circle->pos.y += (circle->res_vel.y / circle->res_vel.x) * (SCRN_W - (circle->pos.x + circle->radius));
		
		// Next, adjust the x position.
		circle->pos.x = (SCRN_W - circle->radius);
		
		// Only reverse the vel if it is moving towards the right.
		if (circle->res_vel.x > 0)
		{
			circle->res_vel.x = -(circle->res_vel.x * circle->bounciness);
		}
		
		// Update vel with friction coefficient.
		circle->res_vel.x *= FRICTION_COEFF;
	}
}


BOOL CPhysics::checkCircleCircleCollisionInaccurate(const POCircle *circle1, const POCircle *circle2)
{
	float x_dist = pow(circle1->pos.x - circle2->pos.x, 2);
	float y_dist = pow(circle1->pos.y - circle2->pos.y, 2);
	
	// Calculate distance using the standard distance formula without square-root.
	float dist = (x_dist + y_dist);
	
	// Check whether the 2 circles have collided.
	if (dist <= pow(circle1->radius + circle2->radius + CIRCLE_EPSILON, 2))
	{
		return TRUE;
	}
	
	return FALSE;
}


BOOL CPhysics::checkCircleCircleCollision(const POCircle *circle1, const POCircle *circle2)
{
	if (checkCircleCircleMovement(circle1, circle2) == FALSE)
	{
		// circles are moving away from each other, there can be no collision.
		return FALSE;
	}
																		
	
	float x_dist = pow(circle1->pos.x - circle2->pos.x, 2);
	float y_dist = pow(circle1->pos.y - circle2->pos.y, 2);
	
	// Calculate distance using the standard distance formula without square-root.
	float dist = (x_dist + y_dist);
	
	// Check whether the 2 circles have collided.
	if (dist <= pow(circle1->radius + circle2->radius + CIRCLE_EPSILON, 2))
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CPhysics::checkCircleCircleMovement(const POCircle *circle1, const POCircle *circle2)
{
    // Position Vector dotted with the Relative Velocity Vector.
	if ( ((circle2->pos.x - circle1->pos.x) * (circle1->res_vel.x - circle2->res_vel.x) +
		(circle2->pos.y - circle1->pos.y) * (circle1->res_vel.y - circle2->res_vel.y)) > 0 )
	{
		return TRUE;
	}
	
	return FALSE;
}


#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
float CPhysics::circleCircleCollisionTime(POCircle *circle1, POCircle *circle2)
{
	float t = 1000;
	float A, B, C, D, DISC;
	// Breaking down the formula for t.
	A = pow(circle1->res_vel.x, 2) + pow(circle1->res_vel.y, 2) - 2 * circle1->res_vel.x * circle2->res_vel.x + pow(circle2->res_vel.x, 2) - 2 * circle1->res_vel.y * circle2->res_vel.y + pow(circle2->res_vel.y, 2);
	B = -circle1->pos.x * circle1->res_vel.x - circle1->pos.y * circle1->res_vel.y + circle1->res_vel.x * circle2->pos.x + circle1->res_vel.y * circle2->pos.y + circle1->pos.x * circle2->res_vel.x -
	circle2->pos.x * circle2->res_vel.x + circle1->pos.y * circle2->res_vel.y - circle2->pos.y * circle2->res_vel.y;
	C = pow(circle1->res_vel.x, 2) + pow(circle1->res_vel.y, 2) - 2 * circle1->res_vel.x * circle2->res_vel.x + pow(circle2->res_vel.x, 2) - 2 * circle1->res_vel.y * circle2->res_vel.y + pow(circle2->res_vel.y, 2);
	D = pow(circle1->pos.x, 2) + pow(circle1->pos.y, 2) - pow(circle1->radius, 2) - 2 * circle1->pos.x * circle2->pos.x + pow(circle2->pos.x, 2) - 2 * circle1->pos.y * circle2->pos.y +
	pow(circle2->pos.y, 2) - 2 * circle1->radius * circle2->radius - pow(circle2->radius, 2);
	DISC = pow((-2 * B), 2) - 4 * C * D;
	
	// If the discriminent if non negative, a collision will occur and
	// we must compare the time to our current time of collision. We
	// udate the time if we find a collision that has occurd earlier
	//than the previous one.
	if (DISC >= 0)
	{
		// We want the smallest time.
		float temp = min (t, 0.5 * (2 * B - sqrt (DISC)) / A);
		t = min (temp, 0.5 * (2 * B + sqrt (DISC)) / A);
	}
	
	return t;
}

float CPhysics::circleCircleCollisionTime(POCircle objects[], int numObjs)
{
	float t = 1000;
	float A, B, C, D, DISC;
	
	POCircle *circle1;
	POCircle *circle2;
	
	for (int i = 0; i < numObjs; ++i)
	{
		circle1 = &objects[i];
		
		if (circle1->active == FALSE)
		{
			continue;
		}
		
		for (int j = i + 1; j < numObjs; ++j)
		{
			circle2 = &objects[j];
			
			
			if (circle2->active == FALSE)
			{
				continue;
			}
			
			if (checkCircleCircleMovement(circle1, circle2) == TRUE)
			{
				// Breaking down the formula for t.
				A = pow(circle1->res_vel.x, 2) + pow(circle1->res_vel.y, 2) - 2 * circle1->res_vel.x * circle2->res_vel.x + pow(circle2->res_vel.x, 2) - 2 * circle1->res_vel.y * circle2->res_vel.y + pow(circle2->res_vel.y, 2);
				B = -circle1->pos.x * circle1->res_vel.x - circle1->pos.y * circle1->res_vel.y + circle1->res_vel.x * circle2->pos.x + circle1->res_vel.y * circle2->pos.y + circle1->pos.x * circle2->res_vel.x -
				circle2->pos.x * circle2->res_vel.x + circle1->pos.y * circle2->res_vel.y - circle2->pos.y * circle2->res_vel.y;
				C = pow(circle1->res_vel.x, 2) + pow(circle1->res_vel.y, 2) - 2 * circle1->res_vel.x * circle2->res_vel.x + pow(circle2->res_vel.x, 2) - 2 * circle1->res_vel.y * circle2->res_vel.y + pow(circle2->res_vel.y, 2);
				D = pow(circle1->pos.x, 2) + pow(circle1->pos.y, 2) - pow(circle1->radius, 2) - 2 * circle1->pos.x * circle2->pos.x + pow(circle2->pos.x, 2) - 2 * circle1->pos.y * circle2->pos.y +
				pow(circle2->pos.y, 2) - 2 * circle1->radius * circle2->radius - pow(circle2->radius, 2);
				DISC = pow((-2 * B), 2) - 4 * C * D;
			}
			
//			A = pow(circle1->vel.x, 2) + pow(circle1->vel.y, 2) - 2 * circle1->vel.x * circle2->vel.x + pow(circle2->vel.x, 2) - 2 * circle1->vel.y * circle2->vel.y + pow(circle2->vel.y, 2);
//			B = -circle1->pos.x * circle1->vel.x - circle1->pos.y * circle1->vel.y + circle1->vel.x * circle2->pos.x + circle1->vel.y * circle2->pos.y + circle1->pos.x * circle2->vel.x -
//			circle2->pos.x * circle2->vel.x + circle1->pos.y * circle2->vel.y - circle2->pos.y * circle2->vel.y;
//			C = pow(circle1->vel.x, 2) + pow(circle1->vel.y, 2) - 2 * circle1->vel.x * circle2->vel.x + pow(circle2->vel.x, 2) - 2 * circle1->vel.y * circle2->vel.y + pow(circle2->vel.y, 2);
//			D = pow(circle1->pos.x, 2) + pow(circle1->pos.y, 2) - pow(circle1->radius, 2) - 2 * circle1->pos.x * circle2->pos.x + pow(circle2->pos.x, 2) - 2 * circle1->pos.y * circle2->pos.y +
//			pow(circle2->pos.y, 2) - 2 * circle1->radius * circle2->radius - pow(circle2->radius, 2);
//			DISC = pow((-2 * B), 2) - 4 * C * D;
			
			// If the discriminent if non negative, a collision will occur and 
			//  we must compare the time to our current time of collision. We   
			//  udate the time if we find a collision that has occurd earlier   
			//  than the previous one.                                          
			if (DISC >= 0)
			{
				// We want the smallest time.
				float temp = min (t, 0.5 * (2 * B - sqrt (DISC)) / A);
				t = min (temp, 0.5 * (2 * B + sqrt (DISC)) / A);
			}
		}
	}
	
	return t;
}

static const float centerZoneThreshold = 2.0;
void CPhysics::circleCircleCollisionWithThreshold(POCircle *circle1, POCircle *circle2, ePhysicsAxis axis, float threshold)
{
	// Execute collision.
	circleCircleCollision(circle1, circle2);
	
	// Check whether we need to zero out any values.
	if (isWithinThreshold(circle1, circle2, axis, threshold))
	{
		// Check for whether we want to zero out any velocities.
		if (circle1->will_cause_x_zero_out == TRUE)
		{
			circle2->x_vel_zeroed_out = TRUE;
		}
		else if (circle2->will_cause_x_zero_out == TRUE)
		{
			circle1->x_vel_zeroed_out = TRUE;
		}	
		
		if (circle1->will_cause_y_zero_out == TRUE)
		{
			circle2->y_vel_zeroed_out = TRUE;
		}
		else if (circle2->will_cause_y_zero_out == TRUE)
		{
			circle1->y_vel_zeroed_out = TRUE;
		}
		
		// Check for whether the zeroing out will cause a chain reaction.
		if (circle1->will_cause_x_zero_out_chain == TRUE)
		{
			circle2->x_vel_zeroed_out = TRUE;
			circle2->will_cause_x_zero_out_chain = TRUE;
		}
		else if (circle2->will_cause_x_zero_out_chain == TRUE)
		{
			circle1->x_vel_zeroed_out = TRUE;
			circle1->will_cause_x_zero_out_chain = TRUE;
		}
		
		if (circle1->will_cause_y_zero_out_chain == TRUE)
		{
			circle2->y_vel_zeroed_out = TRUE;
			circle2->will_cause_y_zero_out_chain = TRUE;
		}
		else if (circle2->will_cause_y_zero_out_chain == TRUE)
		{
			circle1->y_vel_zeroed_out = TRUE;
			circle1->will_cause_y_zero_out_chain = TRUE;
		}
	}
}

BOOL CPhysics::isWithinThreshold(physicsObject *object1, physicsObject *object2, ePhysicsAxis axis, float threshold)
{
	switch (axis)
	{
		case PHYS_AXIS_X:
		{
			// If the 2nd object is withing object1's threshold, then we will return true indicating that some action may happen.
			// Otherwise, we return false which indicates that nothing should be done.
			if (((object1->pos.x + threshold) > object2->pos.x) && ((object1->pos.x - threshold) < object2->pos.x))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
				
		}
		break;
			
		case PHYS_AXIS_Y:
		{
			// Same as x, but on the y axis.
			if (((object1->pos.y + threshold) > object2->pos.y) && ((object1->pos.y - threshold) < object2->pos.y))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
			
		}
		break;
			
		// Not handling z yet.  May in the future when 3d is supported.
		case PHYS_AXIS_Z:
		case PHYS_AXIS_ALL:
		default:
		{
			return TRUE;
		}
	}
}

void CPhysics::drawCircleCircleCollisionVectors(POCircle *circle1, POCircle *circle2)
{
	if ((circle1->active == FALSE) || (circle2->active == FALSE))
	{
		return;
	}
	
	// First get the line between the two circle centers.
	POLine connector_line;
	connector_line.start = circle1->pos;
	connector_line.end = circle2->pos;
	
	// Now get the two circle vectors pointing at each other from the connector line.
	POLine circle1_vec = connector_line;
	
	// Move the vector to the origin before we normalize.
	circle1_vec.end -= circle1_vec.start;
	circle1_vec.start = Vector3(0.0f, 0.0f, 0.0f);
	
	// Now, normalize the vector.
	normalize(&circle1_vec);
	
	POLine circle2_vec;
	circle2_vec.start = connector_line.end;
	circle2_vec.end = connector_line.start;
	
	// Move the circle 2's vector to the origin before we normalize.
	circle2_vec.end -= circle2_vec.start;
	circle2_vec.start = Vector3(0.0f, 0.0f, 0.0f);
	
	// Normalize circle 2's vector.
	normalize(&circle2_vec);
	
	// Now set them to the length of the corresponding radii.
	circle1_vec.end *= circle1->radius;
	circle2_vec.end *= circle2->radius;
	
	// Move the vectors to the positions of the circles.
	circle1_vec.start += circle1->pos;
	circle1_vec.end += circle1->pos;
	circle2_vec.start += circle2->pos;
	circle2_vec.end += circle2->pos;
	
	// Now get the normals of circles vectors.
	POLine *circle1_norm = clock_norm(circle1_vec);
	POLine *circle2_norm = clock_norm(circle2_vec);

	// Now double the length of the normals so that they become tangent lines on the circle's edges.
	float circle1_norm_xdiff = circle1_norm->end.x - circle1_norm->start.x;
	float circle1_norm_ydiff = circle1_norm->end.y - circle1_norm->start.y;
	circle1_norm->start.x -= circle1_norm_xdiff;
	circle1_norm->start.y -= circle1_norm_ydiff;
	
	float circle2_norm_xdiff = circle2_norm->end.x - circle2_norm->start.x;
	float circle2_norm_ydiff = circle2_norm->end.y - circle2_norm->start.y;
	circle2_norm->start.x -= circle2_norm_xdiff;
	circle2_norm->start.y -= circle2_norm_ydiff;
	
	// Swap the points in the normals to make sure they are facing "outward" so that collision occurs clockwise.
	Vector3 temp_point = circle1_norm->end;
	circle1_norm->end = circle1_norm->start;
	circle1_norm->start = temp_point;
	temp_point = circle2_norm->end;
	circle2_norm->end = circle2_norm->start;
	circle2_norm->start = temp_point;
	
	// Move the normals to the ends of the circle vectors that are pointing to each other.
	circle1_norm->start += circle1_vec.end;
	circle1_norm->end += circle1_vec.end;
	circle2_norm->start += circle2_vec.end;
	circle2_norm->end += circle2_vec.end;
	
	// Using the circle vectors and normals, run a circle line collision test on each other.
	//circleLineCollision(circle2_norm, circle1);
	//circleLineCollision(circle1_norm, circle2);
	
	drawCircleLineCollisionVectors(circle2_norm, circle1);
	CGraphics::drawLine(*circle1_norm);
	CGraphics::drawLine(*circle2_norm);

	//CGraphics::drawLine(circle1_vec);
	//CGraphics::drawLine(circle2_vec);
	
	delete circle1_norm;
	circle1_norm = NULL;
	delete circle2_norm;
	circle2_norm = NULL;
}

void CPhysics::circleCircleCollision(POCircle *circle1, POCircle *circle2)
{
//	if ((circle1->active == FALSE) || (circle2->active == FALSE))
//	{
//		return;
//	}
//
//	if (checkCircleCircleMovement(circle1, circle2) == FALSE)
//	{
//		return;
//	}
//	
//	// Check whether the circles will intersect with each other in the next frame.
//	POCircle temp_circle1;
//	temp_circle1.pos.x = circle1->pos.x + (circle1->res_vel.x * ((float)TIME_LAST_FRAME / 1000));
//	temp_circle1.pos.y = circle1->pos.y + (circle1->res_vel.y * ((float)TIME_LAST_FRAME / 1000));
//	temp_circle1.radius = circle1->radius;
//	POCircle temp_circle2;
//	temp_circle2.pos.x = circle2->pos.x + (circle2->res_vel.x * ((float)TIME_LAST_FRAME / 1000));
//	temp_circle2.pos.y = circle2->pos.y + (circle2->res_vel.y * ((float)TIME_LAST_FRAME / 1000));
//	temp_circle2.radius = circle2->radius;
//	
//	
//	
//	// First get the line between the two circle centers.
//	POLine connector_line;
//	connector_line.start = temp_circle1.pos;
//	connector_line.end = temp_circle2.pos;
//	
//	// Now get the two circle vectors pointing at each other from the connector line.
//	POLine circle1_vec = connector_line;
//	
//	// Move the vector to the origin before we normalize.
//	circle1_vec.end -= circle1_vec.start;
//	circle1_vec.start = 0.0;
//	
//	// Now, normalize the vector.
//	normalize(&circle1_vec);
//	
//	POLine circle2_vec;
//	circle2_vec.start = connector_line.end;
//	circle2_vec.end = connector_line.start;
//	
//	// Move the circle 2's vector to the origin before we normalize.
//	circle2_vec.end -= circle2_vec.start;
//	circle2_vec.start = 0.0;
//	
//	// Normalize circle 2's vector.
//	normalize(&circle2_vec);
//	
//	// Now set them to the length of the corresponding radii.
//	circle1_vec.end *= temp_circle1.radius;
//	circle2_vec.end *= temp_circle2.radius;
//	
//	// Move the vectors to the positions of the circles.
//	circle1_vec.start += temp_circle1.pos;
//	circle1_vec.end += temp_circle1.pos;
//	circle2_vec.start += temp_circle2.pos;
//	circle2_vec.end += temp_circle2.pos;
//	
//	// Now get the normals of circles vectors.
//	POLine *circle1_norm = clock_norm(circle1_vec);
//	POLine *circle2_norm = clock_norm(circle2_vec);
//
//	// Now double the length of the normals so that they become tangent lines on the circle's edges.
//	float circle1_norm_xdiff = circle1_norm->end.x - circle1_norm->start.x;
//	float circle1_norm_ydiff = circle1_norm->end.y - circle1_norm->start.y;
//	circle1_norm->start.x -= circle1_norm_xdiff;
//	circle1_norm->start.y -= circle1_norm_ydiff;
//	
//	float circle2_norm_xdiff = circle2_norm->end.x - circle2_norm->start.x;
//	float circle2_norm_ydiff = circle2_norm->end.y - circle2_norm->start.y;
//	circle2_norm->start.x -= circle2_norm_xdiff;
//	circle2_norm->start.y -= circle2_norm_ydiff;
//	
//	// Swap the points in the normals to make sure they are facing "outward" so that collision occurs clockwise.
//	Vector3 temp_point = circle1_norm->end;
//	circle1_norm->end = circle1_norm->start;
//	circle1_norm->start = temp_point;
//	temp_point = circle2_norm->end;
//	circle2_norm->end = circle2_norm->start;
//	circle2_norm->start = temp_point;
//	
//	// Move the normals to the ends of the circle vectors that are pointing to each other.
//	circle1_norm->start += circle1_vec.end;
//	circle1_norm->end += circle1_vec.end;
//	circle2_norm->start += circle2_vec.end;
//	circle2_norm->end += circle2_vec.end;
//	
//
//	// If the circle will collide in the next frame, figure out where it will collide and move it there.
//	// First, get the normal if the line.
//	POLine *line_norm = clock_norm(*circle2_norm);
//	
//	// Normalize the normal line.
//	normalize(line_norm);
//	
//	POLine circle_norm;
//	// Get the inverse of the line norm and place it at the circle's origin.
//	circle_norm.start = line_norm->start * -1;
//	circle_norm.end = line_norm->end * -1;
//	
//	// Now set the length of the circle vector to the radius length.
//	circle_norm.end *= temp_circle1.radius;
//	
//	// Move the inverse normal to the circle's position.
//	circle_norm.start += temp_circle1.pos;
//	circle_norm.end += temp_circle1.pos;
//	
//	// Now check for an intersection between the normal vector and the line.
//	if (checkLineLineIntersection(&circle_norm, circle2_norm))
//	{
//		// Get the intersection point.
//		Vector3 *intersection_point = getLineLineIntersectionPoint(&circle_norm, circle2_norm);
//		
//		// Take the point of intersection and subtract it from the end of the circle where the intersection occured (line_norm.end).
//		Vector3 intersection_diff;
//		intersection_diff.x = intersection_point->x - circle_norm.end.x;
//		intersection_diff.y = intersection_point->y - circle_norm.end.y;
//		
//		// Now add the difference to the line_norm itself to reposition it so that it is just touching the line.
//		circle_norm.start += intersection_diff;
//		circle_norm.end += intersection_diff;
//		
//		// The start of the line_norm is now where the center of the circle should be.  Set the circle position.
//		circle1->pos = circle_norm.start;
//		
//		// Calculate the area of the ball.
//		float circle1_area = FLOAT_PI * pow(circle1->radius, 2);
//		float circle2_area = FLOAT_PI * pow(circle2->radius, 2);
//		
//		// The normalized vector in the x direction.
//		float nx = (circle1->pos.x - circle2->pos.x) / (circle1->radius + circle2->radius);
//		// The normalized vector in the y direction.
//		float ny = (circle1->pos.y - circle2->pos.y) / (circle1->radius + circle2->radius);
//		// The 1st circle's impulse.
//		float a1 = circle1->res_vel.x * nx + circle1->res_vel.y * ny;
//		// The 2nd circle's impulse. 
//		float a2 = circle2->res_vel.x * nx + circle2->res_vel.y * ny;
//		// The resultant impulse.
//		float p = 2 * (a1 - a2) / (circle1_area + circle2_area);
//		
//		// Circle 1's resultant vel values.
//		circle1->res_vel.x = circle1->res_vel.x - p * nx * circle2_area;
//		circle1->res_vel.y = circle1->res_vel.y - p * ny * circle2_area;
//		circle1->res_vel *= circle1->bounciness;
//		
//		// Circle 2's resultant vel values.
//		circle2->res_vel.x = circle2->res_vel.x + p * nx * circle1_area;
//		circle2->res_vel.y = circle2->res_vel.y + p * ny * circle1_area;	
//		circle2->res_vel *= circle2->bounciness;		
//	}
//	
//	
//	delete circle1_norm;
//	circle1_norm = NULL;
//	delete circle2_norm;
//	circle2_norm = NULL;
//	
//	delete line_norm;
//	line_norm = NULL;

	
	if ((circle1->active == FALSE) || (circle2->active == FALSE))
	{
		return;
	}
	
	// Calculate the area of the ball.
	float circle1_area = FLOAT_PI * pow(circle1->radius, 2);
	float circle2_area = FLOAT_PI * pow(circle2->radius, 2);
	
	// The normalized vector in the x direction.
	float nx = (circle1->pos.x - circle2->pos.x) / (circle1->radius + circle2->radius);
	// The normalized vector in the y direction.
	float ny = (circle1->pos.y - circle2->pos.y) / (circle1->radius + circle2->radius);
	// The 1st circle's impulse.
	float a1 = circle1->res_vel.x * nx + circle1->res_vel.y * ny;
	// The 2nd circle's impulse. 
	float a2 = circle2->res_vel.x * nx + circle2->res_vel.y * ny;
	// The resultant impulse.
	float p = 2 * (a1 - a2) / (circle1_area + circle2_area);
	
	// Circle 1's resultant vel values.
	circle1->res_vel.x = circle1->res_vel.x - p * nx * circle2_area;
	circle1->res_vel.y = circle1->res_vel.y - p * ny * circle2_area;
	
	// Circle 2's resultant vel values.
	circle2->res_vel.x = circle2->res_vel.x + p * nx * circle1_area;
	circle2->res_vel.y = circle2->res_vel.y + p * ny * circle1_area;	
}


void CPhysics::applyImpulse(physicsObject *object, float x, float y)
{
	object->res_vel.x += x;
	object->res_vel.y += y;
}

void CPhysics::applyImpulse(physicsObject *object, Vector2 impulse)
{
	applyImpulse(object, impulse.x, impulse.y);
}

void CPhysics::applyImpulse(physicsObject *object, float x, float y, float z)
{
	object->res_vel.x += x;
	object->res_vel.y += y;
	object->res_vel.z += z;
}

void CPhysics::applyImpulse(physicsObject *object, Vector3 impulse)
{
	applyImpulse(object, impulse.x, impulse.y, impulse.z);
}


BOOL CPhysics::checkCircleRayCollision(const POCircle *circle, const POLine *line)
{
	return checkCircleRayCollision(line, circle);
}

BOOL CPhysics::checkCircleRayCollision(const POLine *line, const POCircle *circle)
{
	// Save off the radius for easier to read equations.
	float r = circle->radius;
	
	// Calculate direction of the vector of the ray.
	Vector2 d;
	d.x = line->end.x - line->start.x;
	d.y = line->end.y - line->start.y;
	
	// Calculate the vector from the center circle to ray start.
	Vector2 f;
	f.x = line->start.x - circle->pos.x;
	f.y = line->start.y - circle->pos.y;
	
	// Solve the quadradic equation.
	float a = dotProd(d, d);
	float b = 2 * dotProd(f, d);
	float c = dotProd(f, f) - (r * r);
	
	// Calculate the discriminant.
	float disc = b * b - 4 * a * c;
	
	if (disc < 0)
	{
		// No intersection has occured.
		return FALSE;
	}
	else
	{
		// The ray did not miss the circle.
		// Calculate whether it really hit the ray.
		disc = sqrt(disc);
		float t1 = (-b + disc) / (2 * a);
		float t2 = (-b - disc) / (2 * a);
		
		// Check the first point.
		if (t1 >= 0 && t1 <= 1)
		{
			// Collision occured.
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
		// Now check for the second point.
		if (t2 >= 0 && t2 <= 1)
		{
			// Collision occured.
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}


// Return sthe dot product of the vectors passed in.
float CPhysics::dotProd(Vector2 p1, Vector2 p2)
{
	return ((p1.x * p2.x) + (p1.y * p2.y));
}


// Return sthe dot product of the vectors passed in.
float CPhysics::dotProd(Vector3 p1, Vector3 p2)
{
	return ((p1.x * p2.x) + (p1.y * p2.y) + (p1.z * p2.z));
}


// Returns a line object normal to the one passed in.  The result is in clockwise order.
// This means the if the start point is on the bottom and the end point is above it, the normal line will point to the left.
// However, since our origin is located in the upper left corner of the screen, we need to reverse this.
POLine* CPhysics::clock_norm(POLine line)
{
	float dx = line.end.x - line.start.x;
	float dy = line.end.y - line.start.y;
	
	POLine* norm_line = new POLine;
	norm_line->start.x = 0;
	norm_line->start.y = 0;
	
	// This is what it would be in a normal cartesian coordinate system with the origin in the middle.
	//norm_line->end.x = -dy;
	//norm_line->end.y = dx;
	
	// This is the reversed value since our origin is in the upper left hand corner.
	norm_line->end.x = dy;
	norm_line->end.y = -dx;
	
	return norm_line;
}

float CPhysics::line_slope(POLine line)
{
	return (line.start.y - line.end.y) / (line.start.x - line.end.x);
}

// Returns the length of the given line object.
float CPhysics::line_len(POLine line)
{
	float dx = line.end.x - line.start.x;
	float dy = line.end.y - line.start.y;
	
	return sqrt((dx * dx) + (dy * dy));
}

// Normalizes the line passed in.
void CPhysics::normalize(POLine* line)
{
	float linelen = line_len(*line);
	
	line->end.x /= linelen;
	line->end.y /= linelen;
}


float CPhysics::distBetweenVec(Vector3 vec1, Vector3 vec2)
{
	float dx = vec1.x - vec2.x;
	float dy = vec1.y - vec2.y;
	float dz = vec1.z - vec2.z;
	
	return sqrt((dx * dx) + (dy * dy) + (dz * dz));
}

								
float CPhysics::distBetweenPO(physicsObject obj1, physicsObject obj2)
{
	float dx = obj1.pos.x - obj2.pos.x;
	float dy = obj1.pos.y - obj2.pos.y;
	float dz = obj1.pos.z - obj2.pos.z;
	
	return sqrt((dx * dx) + (dy * dy) + (dz * dz));
}
								

void CPhysics::drawCircleLineCollisionVectors(POLine *line, POCircle *circle)
{
	// Check whether the circle will intersect with the line segment (ray) in the next frame.
	POCircle temp_circle;
	temp_circle.pos.x = circle->pos.x + (circle->res_vel.x * ((float)TIME_LAST_FRAME / 1000));
	temp_circle.pos.y = circle->pos.y + (circle->res_vel.y * ((float)TIME_LAST_FRAME / 1000));
	temp_circle.radius = circle->radius;
	
	
	// If the circle will collide in the next frame, figure out where it will collide and move it there.
	
	// First, get the normal if the line.
	POLine *line_norm = clock_norm(*line);
	
	// Normalize the normal line.
	normalize(line_norm);
	
	// Get the inverse of the line norm and place it at the circle's origin.
	line_norm->start *= -1;
	line_norm->end *= -1;

	// Now set the length of the normal vector to the radius length.
	line_norm->end *= temp_circle.radius;
	
	// Moving the inverse normal to the circle's position.
	line_norm->start += temp_circle.pos;
	line_norm->end += temp_circle.pos;
	
	// Now check for an intersection between the normal vector and the line.
	if (checkLineLineIntersection(line_norm, line))
	{
		CGraphics::drawLine(*line_norm);
	}
	
	
	delete line_norm;
	line_norm = NULL;
}

void CPhysics::circleLineCollision(POCircle *circle, POLine* line)
{
	circleLineCollision(line, circle);
}
			 
			 
void CPhysics::circleLineCollision(POLine *line, POCircle *circle)
{
	// Check whether the circle will intersect with the line segment (ray) in the next frame.
	POCircle temp_circle;
	temp_circle.pos.x = circle->pos.x + (circle->res_vel.x * ((float)TIME_LAST_FRAME / 1000));
	temp_circle.pos.y = circle->pos.y + (circle->res_vel.y * ((float)TIME_LAST_FRAME / 1000));
	temp_circle.radius = circle->radius;
	
	// If the circle will collide in the next frame, figure out where it will collide and move it there.
	
	// First, get the normal if the line.
	POLine *line_norm = clock_norm(*line);
	
	// Normalize the normal line.
	normalize(line_norm);
	
	POLine circle_norm;
	// Get the inverse of the line norm and place it at the circle's origin.
	circle_norm.start = line_norm->start * -1;
	circle_norm.end = line_norm->end * -1;
	
	// Now set the length of the circle vector to the radius length.
	circle_norm.end *= temp_circle.radius;
	
	// Move the inverse normal to the circle's position.
	circle_norm.start += temp_circle.pos;
	circle_norm.end += temp_circle.pos;
	
	// Now check for an intersection between the normal vector and the line.
	if (checkLineLineIntersection(&circle_norm, line))
	{
		// Get the intersection point.
		Vector3 *intersection_point = getLineLineIntersectionPoint(&circle_norm, line);
		
		// Take the point of intersection and subtract it from the end of the circle where the intersection occured (line_norm.end).
		Vector3 intersection_diff;
		intersection_diff.x = intersection_point->x - circle_norm.end.x;
		intersection_diff.y = intersection_point->y - circle_norm.end.y;
		
		// Now add the difference to the line_norm itself to reposition it so that it is just touching the line.
		circle_norm.start += intersection_diff;
		circle_norm.end += intersection_diff;
		
		// The start of the line_norm is now where the center of the circle should be.  Set the circle position.
		circle->pos = circle_norm.start;
		
		// Make the circle bounce off the line.
		
		// Need to calculate the reflection vector of the circle.
		Vector3 wall_norm;
		wall_norm.x =  line_norm->end.x;
		wall_norm.y =  line_norm->end.y;

		// Make the circle react with the reflection vector.
		circle->res_vel -= wall_norm * (dotProd(wall_norm, circle->res_vel) * 2);
		
		// Apply circle properties.
		circle->res_vel *= circle->bounciness;
		
		delete intersection_point;
		intersection_point = NULL;
		
		// Check for whether we want to zero out any velocities.
		if (line->will_cause_x_zero_out == TRUE)
		{
			circle->x_vel_zeroed_out = TRUE;
		}
		if (line->will_cause_y_zero_out == TRUE)
		{
			circle->y_vel_zeroed_out = TRUE;
		}
		
		
		// Check for whether the zeroing out will cause a chain reaction.
		if (line->will_cause_x_zero_out_chain == TRUE)
		{
			circle->x_vel_zeroed_out = TRUE;
			circle->will_cause_x_zero_out_chain = TRUE;
		}
		if (line->will_cause_y_zero_out_chain == TRUE)
		{
			circle->y_vel_zeroed_out = TRUE;
			circle->will_cause_y_zero_out_chain = TRUE;
		}
	}
	
	delete line_norm;
	line_norm = NULL;
}


BOOL CPhysics::checkLineLineIntersection(const POLine *line1, const POLine *line2)
{
	float x1 = line1->start.x;
	float y1 = line1->start.y;
	float x2 = line1->end.x;
	float y2 = line1->end.y;
	
	float x3 = line2->start.x;
	float y3 = line2->start.y;
	float x4 = line2->end.x;
	float y4 = line2->end.y;
	
	float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	
	// When the demonimator is zero, it means that the 2 lines are parallel, thus never intersecting.
	// This also means that the 2 line lie on one another.  We're not interested in this result.
	if (denom == 0.0)
	{
		return FALSE;
	}
	
	float eq1 = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
	float eq2 = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
	
	if ( (eq1 > 0) && (eq1 < 1) && (eq2 > 0) && (eq2 < 1) )
	{
		return TRUE;		
	}
	
	return FALSE;
}

Vector3* CPhysics::getLineLineIntersectionPoint(POLine *line1, POLine *line2)
{
	float x1 = line1->start.x;
	float y1 = line1->start.y;
	float x2 = line1->end.x;
	float y2 = line1->end.y;
	
	float x3 = line2->start.x;
	float y3 = line2->start.y;
	float x4 = line2->end.x;
	float y4 = line2->end.y;
	
	float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	
	if (denom == 0.0)
	{
		DPRINT_PHYSICS("getLineLineIntersectionPoint failed, lines do not intersect");
		return NULL;
	}
	
	float eq1 = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
	//float eq2 = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
	
	Vector3 *intersection_point = new Vector3;
	intersection_point->x = x1 + eq1 * (x2 - x1);
	intersection_point->y = y1 + eq1 * (y2 - y1);
	
	return intersection_point;
}