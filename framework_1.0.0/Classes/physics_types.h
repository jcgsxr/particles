/*
 *  physics_types.h
 *  framework
 *
 *  Created by Johnny Chan on 12/11/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */


#ifndef __PHYSICSTYPES_H__
#define __PHYSICSTYPES_H__

#include "Vector3.h"


typedef enum ePhysicsTypes
{
	ePhysicsTypeNone = 0,
	ePhysicsTypeCircle,
	ePhysicsTypeLine,
	ePhysicsTypeRect,
	ePhysicsTypeGravWell,
} ePhysicsTypes;


//////////////////////////////////////////////////////////////////////////////////////////
// Objects that can be used with the CPhysics class.

/*! \struct physicsObject
 *	\brief The Physics Object type.
 *
 * A physics object is used mainly as part of a CGameObject and enables a game object to have physics run on it.
 */
typedef struct physicsObject 
{
	// We use bool here instead of BOOL because I don't want to have to include "types.h" here.
	bool active;	// Indicates whether or not we want to run physics on this object.
	
	// Holds the type of the physics object.
	ePhysicsTypes type;
	
	Vector3 vel;		// The velocity that is used to update the object position.
	Vector3 res_vel;	// The velocity value that calculation are done on.  vel is set to res_vel upon a call to update.
	Vector3 vel_cap;	// A cap on the velocity on this object.
	Vector3 acc;		// The acceleration of this object.
	Vector3 pos;		// The position of this object.
	Vector3 prev_pos;	// The previous position of this object from the previous immediate frame.
	
	Vector3 vel_release_threshold;	// When the object reaches this point, velocity will be un-zeroed out if it was previously zeroed out.
	
	float weight;		// Affects collision reaction energy with other objects.
	float bounciness;	// Affects energy loss when reacting with collision.  1.0 is energy lossless collision.
	float elasticity;	// 
	float hardness;		//
	float edginess;		// Affects the randomness of the x velocity after landing on a surface.  0.0 is zero randomness.
	float resistance;	// An air friction value. Velocity is multiplied by this value if it is less than 1.0.
	
	bool x_vel_zeroed_out;	// Indicates whether the x velocity is locked.
	bool y_vel_zeroed_out;	// Indicates whether the y velocity is locked.
	bool z_vel_zeroed_out;	// Indicates whether the z velocity is locked.
	
	bool will_cause_x_zero_out; // If true, any object colliding with this object will have it's x velocity zeroed out.
	bool will_cause_y_zero_out; // If true, any object colliding with this object will have it's y velocity zeroed out.
	bool will_cause_z_zero_out; // If true, any object colliding with this object will have it's z velocity zeroed out.
	
	bool will_cause_x_zero_out_chain;	// If true, will cause the object that collides with this one also have their will_cause_x_zero_out_chain to be set to TRUE.
	bool will_cause_y_zero_out_chain;	// If true, will cause the object that collides with this one also have their will_cause_y_zero_out_chain to be set to TRUE.
	bool will_cause_z_zero_out_chain;	// If true, will cause the object that collides with this one also have their will_cause_z_zero_out_chain to be set to TRUE.
	
	// IMPORTANT: Usage movement state and physics rewinding is only reliable when velocity is constant!
	int movement_state;		// This is a positive 1 when updating physics in a normal forward-in-time manner. It is set to -1 when we want to rewind physics.
	long physics_counter;	// Used to store the number of physics updates this object has been through. Used to make sure we don't ever rewind past the beginning update.
	
	// Overloading operators for the various physics objects.
	
	void operator = (physicsObject obj)
	{
		active = obj.active;
		vel = obj.vel;
		res_vel = obj.res_vel;
		acc = obj.acc;
		pos = obj.pos;
		
		x_vel_zeroed_out = obj.x_vel_zeroed_out;
		y_vel_zeroed_out = obj.y_vel_zeroed_out;
		z_vel_zeroed_out = obj.z_vel_zeroed_out;
		
		will_cause_y_zero_out = obj.will_cause_y_zero_out;
		will_cause_x_zero_out = obj.will_cause_x_zero_out;
		will_cause_z_zero_out = obj.will_cause_z_zero_out;
		
		will_cause_y_zero_out_chain = obj.will_cause_y_zero_out_chain;
		will_cause_x_zero_out_chain = obj.will_cause_x_zero_out_chain;
		will_cause_z_zero_out_chain = obj.will_cause_z_zero_out_chain;
	}
} physicsObject;

/*! \struct POCircle
 *	\brief The Circle Physics Object type.
 *
 * A circle physics object is a physicsObject with a radius value.
 */
typedef struct POCircle : physicsObject
{
	float radius;	
	
	void operator = (POCircle obj)
	{
		radius = obj.radius;
		
		active = obj.active;
		vel = obj.vel;
		res_vel = obj.res_vel;
		acc = obj.acc;
		pos = obj.pos;
		
		x_vel_zeroed_out = obj.x_vel_zeroed_out;
		y_vel_zeroed_out = obj.y_vel_zeroed_out;
		z_vel_zeroed_out = obj.z_vel_zeroed_out;
		
		will_cause_y_zero_out = obj.will_cause_y_zero_out;
		will_cause_x_zero_out = obj.will_cause_x_zero_out;
		will_cause_z_zero_out = obj.will_cause_z_zero_out;
		
		will_cause_y_zero_out_chain = obj.will_cause_y_zero_out_chain;
		will_cause_x_zero_out_chain = obj.will_cause_x_zero_out_chain;
		will_cause_z_zero_out_chain = obj.will_cause_z_zero_out_chain;
	}
	
}POCircle;

/*! \struct POLine
 *	\brief The Line Physics Object type.
 *
 * A line physics object is a physicsObject with a start and end point.
 */
typedef struct POLine : physicsObject
{
	Vector3 start;
	Vector3 end;
	
	void operator = (POLine obj)
	{
		start = obj.start;
		end = obj.end;
		
		active = obj.active;
		vel = obj.vel;
		res_vel = obj.res_vel;
		acc = obj.acc;
		pos = obj.pos;
		
		x_vel_zeroed_out = obj.x_vel_zeroed_out;
		y_vel_zeroed_out = obj.y_vel_zeroed_out;
		z_vel_zeroed_out = obj.z_vel_zeroed_out;
		
		will_cause_y_zero_out = obj.will_cause_y_zero_out;
		will_cause_x_zero_out = obj.will_cause_x_zero_out;
		will_cause_z_zero_out = obj.will_cause_z_zero_out;
		
		will_cause_y_zero_out_chain = obj.will_cause_y_zero_out_chain;
		will_cause_x_zero_out_chain = obj.will_cause_x_zero_out_chain;
		will_cause_z_zero_out_chain = obj.will_cause_z_zero_out_chain;
	}
}POLine;

/*! \struct PORect
 *	\brief The Rectangle Physics Object type.
 *
 * A rectangle physics object is a physicsObject with a width and height.
 */
typedef struct PORect : physicsObject
{
	float width;
	float height;
	
	void operator = (PORect obj)
	{
		width = obj.width;
		height = obj.height;
		
		active = obj.active;
		vel = obj.vel;
		res_vel = obj.res_vel;
		acc = obj.acc;
		pos = obj.pos;
		
		x_vel_zeroed_out = obj.x_vel_zeroed_out;
		y_vel_zeroed_out = obj.y_vel_zeroed_out;
		z_vel_zeroed_out = obj.z_vel_zeroed_out;
		
		will_cause_y_zero_out = obj.will_cause_y_zero_out;
		will_cause_x_zero_out = obj.will_cause_x_zero_out;
		will_cause_z_zero_out = obj.will_cause_z_zero_out;
		
		will_cause_y_zero_out_chain = obj.will_cause_y_zero_out_chain;
		will_cause_x_zero_out_chain = obj.will_cause_x_zero_out_chain;
		will_cause_z_zero_out_chain = obj.will_cause_z_zero_out_chain;
	}
}PORect;


/*! \struct POGravWell
 *	\brief The Gravity Well Physics Object type.
 *
 * A gravity well is a special type of physics object where other physics objects are either attracted to or repelled by.
 */
typedef struct POGravWell : physicsObject
{
	float grav_const;	// The gravity constant is used to determine the power of the gravity well.
	bool is_attract;	// Indicates whether the gravity well will attract or repel other physics objects.
}POGravWell;

#endif
