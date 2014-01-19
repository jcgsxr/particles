/*
 *  Physics.h
 *  framework
 *
 *  Created by Johnny Chan on 12/5/09.
 *  Copyright 2009 LlamaFace. All rights reserved.
 *
 */

#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "types.h"
#include "UniversalScale.h"

static const int PHYSICS_DELAY_MS = 16; //(1000 / 60);		/*!< A frame delay time for the physics update. This is used to help calculate how many physics updates need to occur in one frame. */
static const float PHYSICS_DELAY = 60.0f / 1000.0f;		/*!< This is multiplied with the physics object velocity and counts as one update. */
//static float CHANGE_DELAY = PHYSICS_DELAY;		/*!< A global physics property that can be used to aid in implementing a physics delay. */
static const float GRAVITY = pixel(9.8);				/*!< Gravity value measured in pixels per second. */
static const float ACCELERATION = pixel(9.8 / 1000);	/*!< Acceleration value measured in pixels per second per second. */
static const float FRICTION_COEFF = 0.98;		/*!< A friction coefficient used for energy degredation. */
static const float CIRCLE_EPSILON = .01;		/*!< The max distance that two circles can be apart and still be considered colliding. */
static const float VELOCITY_EPSILON = .001;		/*!< When an object's velocity reaches anything less than this value, we zero out the velocity. */

typedef enum ePhysicsMovementState
{
	PHYSICS_MOVEMENT_FORWARD = 1,
	PHYSICS_MOVEMENT_REWIND = -1,
}ePhysicsMovementState;

/*! \class CPhysics
 * \brief The Physics class.
 *
 * The Physics class is responsible for running physics collisions and reactions on physics objects.
 */
class CPhysics
{
public:
	
	/*! \enum ePhysicsAxis
	 *	\brief Used for functions that manipulate a specific axis.
	 *
	 * When running physics on a physics object, it is possible to restrict physics to be run on certain axes. This is useful when there are objects that only require reaction and collision on certain axes.
	 * As a utility class, all physics methods are accessible globally.
	 */
	typedef enum ePhysicsAxis
	{
		PHYS_AXIS_X = 0x0001,	/*!< The x axis. */
		PHYS_AXIS_Y = 0x0002,	/*!< the y axis. */
		PHYS_AXIS_Z = 0x0004,	/*!< The z axis. */
		PHYS_AXIS_ALL = (PHYS_AXIS_X | PHYS_AXIS_Y | PHYS_AXIS_Z),	/*!< All axes, or regular physics. */
	} ePhysicsAxis;

	/*! \fn initPhysicsObject(physicsObject *object)
	 *  \brief Initializes all physics properties of the physics object.
	 *  
	 *	\param object The physics object to initialize.
	 *  \return n/a
	 */
	static void initPhysicsObject(physicsObject *object);
	
	/*! \fn initPhysicsObject(physicsObject *object, float x, float y)
	 *  \brief Initializes the object and sets the position to the given x and y coordinates.
	 *  
	 *	\param object The physics object to initialize.
	 *	\param x The x location of the physics object.
	 *	\param y the y location of the physics object.
	 *  \return n/a
	 */
	static void initPhysicsObject(physicsObject *object, float x, float y);
	
	/*! \fn initCircleObject(POCircle *object, float radius)
	 *  \brief Sets the physics type to circle and then initializes all other properties.
	 *  
	 *	\param object The physics object to initialize.
	 *	\param radius The radius of the physics object.
	 *  \return n/a
	 */
	static void initCircleObject(POCircle *object, float radius);
	
	/*! \fn initLineObject(POLine *object, float startX, float startY, float endX, float endY)
	 *  \brief Sets the physics type to line and then initializes all other properties.
	 *  
	 *	\param object The physics object to initialize.
	 *	\param startX The x component of the first point of the line.
	 *	\param startY The y component of the first point of the line.
	 *	\param endX The x component of the second point of the line.
	 *	\param endY The y component of the second point of the line.
	 *  \return n/a
	 */
	static void initLineObject(POLine *object, float startX, float startY, float endX, float endY);

	/*! \fn initGravWellObject(POGravWell *gravWell, float gravConst)
	 *  \brief Sets the physics type to gravity well and initialized all other physics properties.
	 *  
	 *	\param object The physics object to initialize.
	 *	\param gravConst The gravity constant, used to help determine the power of gravity that this object exerts.
	 *  \return n/a
	 */
	static void initGravWellObject(POGravWell *gravWell, float gravConst);
	
	/*! \fn updateCollisionsAndPhysics(physicsObject **objects, int numObjects, float threshold)
	 *  \brief Given a set of physics objects, updates all collisions and physics between them.
	 *  
	 *	\param objects The set of physics objects to run collisions on.
	 *	\param numObjects The number of physics objects in the set.
	 *	\param threshold This value indicates that certain actions will occur after a collision only if the objects are within a threshold distance of each other. This is useful for having a certain axis react when a collision of a different axis occurs.
	 *  \return n/a
	 */
	static void updateCollisionsAndPhysics(physicsObject **objects, int numObjects, float threshold = 0.0);

	/*! \fn checkObjectCollisions(const physicsObject *obj1, const physicsObject *obj2)
	 *  \brief Given any type of pair of physics objects, runs a collision check between them.
	 *  
	 *	\param obj1 The first object to run a collision check on.
	 *	\param obj2 The second object to run a collision check on.
	 *  \return TRUE if a collision has occured, FALSE otherwise.
	 */
	static BOOL checkObjectCollisions(const physicsObject *obj1, const physicsObject *obj2);

	/*! \fn runObjectCollisionReactions(physicsObject *obj1, physicsObject *obj2)
	 *  \brief Given any type of pair of physics objects, run the collision reaction between them.
	 *  
	 *	\param obj1 The first object to run a collision reaction on.
	 *	\param obj2 The second object to run a collision reaction on.
	 *  \return n/a
	 */
	static void runObjectCollisionReactions(physicsObject *obj1, physicsObject *obj2);

	/*! \fn runObjectCollisionReactionsWithThreshold(physicsObject *obj1, physicsObject *obj2, ePhysicsAxis axis, float threshold)
	 *  \brief Runs collision reactions between the two objects with some extra parameters.
	 *  
	 *	\param obj1 The first object to run a collision reaction on.
	 *	\param obj2 The second object to run a collision reaction on.
	 *	\param axis The axis that the collision reaction will be executed.
	 *	\param threshold The threshold distance value that the collision must be within in order for a reaction to occur.
	 *  \return n/a
	 */
	static void runObjectCollisionReactionsWithThreshold(physicsObject *obj1, physicsObject *obj2, ePhysicsAxis axis, float threshold);

	/*! \fn resetVelocity(physicsObject *object)
	 *  \brief Resets the object's velocity to zero.
	 *  
	 *	\param object The physics object who's velocity will be set to 0.
	 *  \return n/a
	 */
	static void resetVelocity(physicsObject *object);

	/*! \fn updatePhysics(physicsObject *object)
	 *  \brief This runs physics on the object with calculated time last frame #TIME_LAST_FRAME.
	 *  
	 *	\param object The physics object to run update on.
	 *  \return n/a
	 */
	static void updatePhysics(physicsObject *object, bool enableFrameSkip = TRUE);

	/*! \fn updatePhysics(physicsObject *object, float time)
	 *  \brief This runs physics on the object with custom frame time.
	 *  
	 *	\param object The physics object to run update on.
	 *	\param time The frame time.
	 *  \return n/a
	 */
	static void updatePhysics(physicsObject *object, float time, bool enableFrameSkip = TRUE);

	/*! \fn updateGravWell(const POGravWell *gravWell, physicsObject *object)
	 *  \brief Applies the force of the given gravity well on the given physics object.
	 *  
	 *	\param gravWell The gravity well.
	 *	\param object The physics object to apply forces on.
	 *  \return n/a
	 */
	static void updateGravWell(const POGravWell *gravWell, physicsObject *object, bool enableFrameSkip = TRUE);
	
	static void updateGravWell(const POGravWell *gravWell, physicsObject *object, float time, bool enableFrameSkip = TRUE);
	
	/*! \fn updateAcceleration(physicsObject *object)
	 *  \brief Updates the physics object's velocity with it's acceleration.
	 *  
	 *	\param object The physics object that will have it's velocity updated with it's acceleration.
	 *  \return n/a
	 */
	static void updateAcceleration(physicsObject *object);

	/*! \fn circleScreenCollision(POCircle *circle)
	 *  \brief Runs physics between the screen borders and the given circle.
	 *  
	 *	\param circle The circle physics object to run collisions on.
	 *  \return n/a
	 */
	static void circleScreenCollision(POCircle *circle);

	/*! \fn checkCircleCircleCollision(const POCircle *circle1, const POCircle *circle2)
	 *  \brief Runs a circle to circle physics object collision check.
	 *  
	 *	\param circle1 The first circle to run the collision check on.
	 *	\param circle2 The second circle to run the collision check on.
	 *  \return TRUE if the circles have collided, FALSE otherwise.
	 */
	static BOOL checkCircleCircleCollision(const POCircle *circle1, const POCircle *circle2);

	/*! \fn checkCircleCircleCollisionInaccurate(const POCircle *circle1, const POCircle *circle2)
	 *  \brief A less accurate but faster check for circle to circle physics object collision check.
	 *  
	 * Used when we don't care whether circles stick to each other or not.
	 *	\param circle1 The first circle to run the collision check on.
	 *	\param circle2 The second circle to run the collision check on.
	 *  \return TRUE if the circles have collided, FALSE otherwise.
	 */
	static BOOL checkCircleCircleCollisionInaccurate(const POCircle *circle1, const POCircle *circle2);

	/*! \fn checkCircleCircleMovement(const POCircle *circle1, const POCircle *circle2)
	 *  \brief A check to see whether two circle physics objects are moving towards on another.
	 *  
	 *	\param circle1 The first circle to run the movement check on.
	 *	\param circle2 The second circle to run the movement check on.
	 *  \return TRUE if the circles are moving towards each other, FALSE otherwise.
	 */
	static BOOL checkCircleCircleMovement(const POCircle *circle1, const POCircle *circle2);

	/*! \fn circleCircleCollision(POCircle *circle1, POCircle *circle2)
	 *  \brief Runs physics between the two circles upon a collision.
	 *  
	 *	\param circle1 The first circle to run a collision reaction on.
	 *	\param circle2 The second circle to run a collision reaction on.
	 *  \return n/a
	 */
	static void circleCircleCollision(POCircle *circle1, POCircle *circle2);

	/*! \fn isWithinThreshold(physicsObject *object1, physicsObject *object2, ePhysicsAxis axis, float threshold)
	 *  \brief Tells whether one circle is within a distance threshold of another.
	 *  
	 *	\param object1 The first object to run the threshold distance check on.
	 *	\param object2 The second object to run the threshold distance check on.
	 *	\param axis The axis to run the threshold distance check on.
	 *	\param threshold The threshold distance.
	 *  \return TRUE if the objects are within the threshold distance of one another, FALSE otherwise.
	 */
	static BOOL isWithinThreshold(physicsObject *object1, physicsObject *object2, ePhysicsAxis axis, float threshold);

	/*! \fn circleCircleCollisionWithThreshold(POCircle *circle1, POCircle *circle2, ePhysicsAxis axis, float threshold)
	 *  \brief Circle to circle collision with extra parameters.
	 *  
	 *	An axis can be specified in order to only collide comparing a specific axis.
	 *	The threshold distance is used if we want to check a collision with a different size than the actual object itself.
	 *	\param circle1 The first circle to run a collision reaction on.
	 *	\param circle2 The second circle to run a collision reaction on.
	 *	\param axis The axis to run the threshold distance check on.
	 *	\param threshold The threshold distance.
	 *  \return n/a
	 */
	static void circleCircleCollisionWithThreshold(POCircle *circle1, POCircle *circle2, ePhysicsAxis axis, float threshold);

	/*! \fn circleCircleCollisionTime(POCircle *circle1, POCircle *circle2)
	 *  \brief Returns the amount of time immediately before a collision occurs between two circle physics objects.
	 *  
	 *	\param circle1 The first circle to run the time check on.
	 *	\param circle2 The second circle to run the time check on.
	 *  \return The amount of time immediately before a collision will occur.
	 */
	static float circleCircleCollisionTime(POCircle *circle1, POCircle *circle2);

	/*! \fn circleCircleCollisionTime(POCircle objects[], int numObjs)
	 *  \brief Returns the smallest amount of time immediately before a collision occurs between a set of circle physics objects.
	 *  
	 *	\param objects The set of circle physics objects to run the time check on.
	 *	\param The number of circle physics objects in the set.
	 *  \return The smallest amount of time immediately before a collision will occur between a set of circle physics objects.
	 */
	static float circleCircleCollisionTime(POCircle objects[], int numObjs);

	/*! \fn applyImpulse(physicsObject *object, Vector2 impulse)
	 *  \brief Applies an impulse on the velocity of the physics object.
	 *  
	 * The impulse passed in will be applied to the object directly.
	 *	\param object The physics object that the impulse will be applied on.
	 *	\param impulse The impulse.
	 *  \return n/a
	 */
	static void applyImpulse(physicsObject *object, Vector2 impulse);
	
	/*! \fn applyImpulse(physicsObject *object, float x, float y)
	 *  \brief Applies an impulse on the velocity of the physics object.
	 *  
	 * The impulse passed in will be applied to the object directly.
	 *	\param object The physics object that the impulse will be applied on.
	 *	\param x The x impulse.
 	 *	\param x The y impulse.
	 *  \return n/a
	 */
	static void applyImpulse(physicsObject *object, float x, float y);
	
	/*! \fn applyImpulse(physicsObject *object, Vector3 impulse)
	 *  \brief Applies an impulse on the velocity of the physics object.
	 *  
	 * The impulse passed in will be applied to the object directly.
	 *	\param object The physics object that the impulse will be applied on.
	 *	\param impulse The impulse.
	 *  \return n/a
	 */
	static void applyImpulse(physicsObject *object, Vector3 impulse);
	
	/*! \fn applyImpulse(physicsObject *object, float x, float y, float z)
	 *  \brief Applies an impulse on the velocity of the physics object.
	 *  
	 * The impulse passed in will be applied to the object directly.
	 *	\param object The physics object that the impulse will be applied on.
	 *	\param x The x impulse.
 	 *	\param x The y impulse.
	 *	\param z The z impulse.
	 *  \return n/a
	 */
	static void applyImpulse(physicsObject *object, float x, float y, float z);

	/*! \fn checkCircleRayCollision(const POLine *line, const POCircle *circle)
	 *  \brief This determines whether a collision between a circle physics object and a line physics object has occured.
	 *  
	 *	\param line The line physics object.
	 *	\param circle The circle physics object.
	 *  \return TRUE if the objects have collided, FALSE otherwise.
	 */
	static BOOL checkCircleRayCollision(const POLine *line, const POCircle *circle);
	
	/*! \fn checkCircleRayCollision(const POCircle *circle, const POLine *line)
	 *  \brief This determines whether a collision between a circle physics object and a line physics object has occured.
	 *  
 	 *	\param circle The circle physics object.
	 *	\param line The line physics object.
	 *  \return TRUE if the objects have collided, FALSE otherwise.
	 */
	static BOOL checkCircleRayCollision(const POCircle *circle, const POLine *line);

	/*! \fn circleLineCollision(POLine *line, POCircle *circle)
	 *  \brief Runs collision response between a line and circle.
	 *  
	 *	\param line The line physics object to run the collision reaction on.
	 *	\param circle The circle physics object to run the collision reaction on.
	 *  \return n/a
	 */
	static void circleLineCollision(POLine *line, POCircle *circle);
	
	/*! \fn circleLineCollision(POCircle *circle, POLine* line)
	 *  \brief Runs collision response between a line and circle.
	 *  
	 *	\param circle The circle physics object to run the collision reaction on.
	 *	\param line The line physics object to run the collision reaction on.
	 *  \return n/a
	 */
	static void circleLineCollision(POCircle *circle, POLine* line);

	/*! \fn drawCircleCircleCollisionVectors(POCircle *circle1, POCircle *circle2)
	 *  \brief Renders a line on the circles representing the collision vector between the two circles.
	 *  
	 * Used for debugging purposes.
	 *	\param circle1 The first circle to run the collision check on.
	 *	\param circle2 The second circle to run the collision check on.
	 *  \return n/a
	 */
	static void drawCircleCircleCollisionVectors(POCircle *circle1, POCircle *circle2);
	
	/*! \fn drawCircleLineCollisionVectors(POLine *line, POCircle *circle)
	 *  \brief Renders a line on the circle and line representing the collision vector between the two objects.
	 *  
	 * Used for debugging purposes.
	 *	\param line The line physics object to run the collision check on.
	 *	\param circle The circle physics object to run the collision check on.
	 *  \return n/a
	 */
	static void drawCircleLineCollisionVectors(POLine *line, POCircle *circle);

	/*! \fn dotProd(Vector2 p1, Vector2 p2)
	 *  \brief Returns the dot product of the vectors passed in.
	 *  
	 *	\param p1 The first vector.
	 *	\param p2 The second vector.
	 *  \return The dot product of the two vectors.
	 */
	static float dotProd(Vector2 p1, Vector2 p2);

	/*! \fn dotProd(Vector3 p1, Vector3 p2)
	 *  \brief Returns the dot product of the vectors passed in.
	 *  
	 *	\param p1 The first vector.
	 *	\param p2 The second vector.
	 *  \return The dot product of the two vectors.
	 */
	static float dotProd(Vector3 p1, Vector3 p2);
	
	/*! \fn clock_norm(POLine line)
	 *  \brief Returns a line object normal to the one passed in.
	 * 
	 * The result is in clockwise order.
	 * This means the if the start point is on the bottom and the end point is above it, the normal line will point to the left.
	 *	\param line The line physics object.
	 *  \return A line physics object that is normal to the line physics object passed in.
	 */
	static POLine* clock_norm(POLine line);

	/*! \fn line_len(POLine line)
	 *  \brief Returns the length of the given line object.
	 *  
	 *	\param line The line physics object.
	 *  \return The length of the line.
	 */
	static float line_len(POLine line);

	/*! \fn line_slope(POLine line)
	 *  \brief Returns the slope of the line.
	 *  
	 *	\param line The line physics object.
	 *  \return The slope value of the line.
	 */
	static float line_slope(POLine line);
	
	/*! \fn normalize(POLine* line)
	 *  \brief Normalizes the line passed in.
	 *  
	 *	\param line The line physics object.
	 *  \return n/a
	 */
	static void normalize(POLine* line);
	
	/*! \fn distBetweenVec(Vector3 vec1, Vector3 vec2)
	 *  \brief Returns the distance between the two vectors.
	 *  
	 *	\param vec1 The first vector used to calculate distance.
 	 *	\param vec2 The second vector used to calculate distance.
	 *  \return n/a
	 */
	static float distBetweenVec(Vector3 vec1, Vector3 vec2);
	
	/*! \fn distBetweenPO(physicsObject obj1, physicsObject obj2)
	 *  \brief Returns the distance between the two physics objects.
	 *  
	 *	\param obj1 The first physics object used to calculate distance.
	 *	\param obj2 The second physics object used to calculate distance.
	 *  \return n/a
	 */
	static float distBetweenPO(physicsObject obj1, physicsObject obj2);	

	/*! \fn checkLineLineIntersection(const POLine *line1, const POLine *line2)
	 *  \brief Checks whether the two lines intersect each other.
	 *  
	 *	\param line1 The first line to run the intersect check on.
	 *	\param line2 The second line to run the intersect check on.
	 *  \return TRUE if the two lines intersect one another, FALSE otherwise.
	 */
	static BOOL checkLineLineIntersection(const POLine *line1, const POLine *line2);

	/*! \fn getLineLineIntersectionPoint(POLine *line1, POLine *line2)
	 *  \brief Returns the point of intersection of the two lines.
	 *  
	 *	\param line1 The first line to run the intersect check on.
	 *	\param line2 The second line to run the intersect check on. 
	 *  \return The point of intersection of the two lines, NULL otherwise.
	 */
	static Vector3* getLineLineIntersectionPoint(POLine *line1, POLine *line2);

};

#endif