/*
 *  Camera.h
 *  Wurd
 *
 *  Created by Johnny Chan on 3/21/10.
 *  Copyright 2010 LlamaFace. All rights reserved.
 *
 */

#ifndef __CAMERASYSTEM_H__
#define __CAMERASYSTEM_H__

#include "types.h"
#include "physics_types.h"

// The camera z start position.
static const int CAMERA_START_Z = 250;
// Used to translate the touch distance into a useable zoom value.
static const float CAMERA_ZOOM_DAMPER = .005;
// Used to translate the touch distance into a useable movement value.
static const float CAMERA_MOVE_DAMPER = .2;
// The lowest zoom value. If changed to be less than zero, everything will be mirror-flipped.
static const float CAMERA_ZOOM_LIMIT = 0.01;

/*! \class CCamera
* \brief The Camera System class.
*
* The Camera System is used in 3D mode to move and reposition the viewable angles and zoom distances of the viewer.
*/
class CCamera
{
public:
	
	void init(void);
	void update(void);
	
	// Set camera's world position.
	float* getViewMatrix(void) {return (float*)_view_mat; }
	
	void reset();
	
	void handleTouch(float x, float y, eTouchPhase phase);
	void handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2);
	
private:
	float _pos[3];
	float _view_mat[16];
		
	Vector3 _touch_pos_prev;
	Vector3 _touch_dist;
	
	float _mult_touch_dist_prev;
	
	// Used to prevent large differences between last touch and current touch locations.
	BOOL _skip_touch;
	BOOL _skip_touch_multi;
};



#endif