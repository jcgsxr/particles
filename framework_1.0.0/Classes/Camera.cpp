/*
 *  Camera.cpp
 *  Wurd
 *
 *  Created by Johnny Chan on 3/21/10.
 *  Copyright 2010 LlamaFace. All rights reserved.
 *
 */

#include "string.h"
#include "Utils.h"
#include "SystemDefines.h"
#include "Camera.h"
#include "MathUtil.h"

void CCamera::init()
{
	memset(&_pos, 0, sizeof(float) * 3);
	memset(&_view_mat, 0, sizeof(float) * 16);
	_touch_pos_prev = Vector3(0.0f, 0.0f, 0.0f);
	_touch_dist = Vector3(0.0f, 0.0f, 0.0f);
	_mult_touch_dist_prev = 0;
	_skip_touch = TRUE;
	_skip_touch_multi = TRUE;
		
	float cam_x, cam_y, cam_z;	
	// Translate the coordinates to the 3d view coordinates.
	coordsScreenTo3D(HALF_SCRN_W, HALF_SCRN_H, CAMERA_START_Z, &cam_x, &cam_y, &cam_z);
	
	// Run update once to set all the world positions and angles.
	_touch_dist.z = cam_z;
	update();
}


void CCamera::reset()
{
	glLoadIdentity();
	gluLookAt(0, 0, 0,
			  0, 0, -1,
			  0, 1, 0);
}


void CCamera::update()
{			
	
	glLoadIdentity();
	
	_pos[0] += _touch_dist.x;
	_pos[1] += _touch_dist.y;
	_pos[2] += _touch_dist.z;
	
	// Don't let z go negative.
	if (_pos[2] < CAMERA_ZOOM_LIMIT)
	{
		_pos[2] = CAMERA_ZOOM_LIMIT;
	}
	
	gluLookAt(0, 0, _pos[2],
			  0, 0, 0,
			  0, 1, 0);
	
	
	//glTranslatef(0, 0, _touch_dist.z);
	glRotatef(_pos[1], 1, 0, 0);
	glRotatef(_pos[0], 0, 1, 0);
	
	//DPRINT_CAMERA("_pos x:%f y:%f z:%f \n", _pos[0], _pos[1], _pos[2]);
	
	
	_touch_dist.zero();

	// Grab the projection matrix so we can update our world position values.
	glGetFloatv(GL_MODELVIEW_MATRIX, _view_mat);

//	DPRINT_CAMERA("===================\n");
//	DPRINT_CAMERA("viewmat %f %f %f %f \n", _view_mat[0], _view_mat[4], _view_mat[8], _view_mat[12]);
//	DPRINT_CAMERA("viewmat %f %f %f %f \n", _view_mat[1], _view_mat[5], _view_mat[9], _view_mat[13]);
//	DPRINT_CAMERA("viewmat %f %f %f %f \n", _view_mat[2], _view_mat[6], _view_mat[10], _view_mat[14]);
//	DPRINT_CAMERA("viewmat %f %f %f %f \n", _view_mat[3], _view_mat[7], _view_mat[11], _view_mat[15]);
//	DPRINT_CAMERA("===================\n");
}


void CCamera::handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
{
	// Handle multi-touch for zooming.
	if (((phase1 == TouchPhaseMoved) || (phase2 == TouchPhaseMoved)) && (!_skip_touch_multi))
	{
		_touch_dist.z += (_mult_touch_dist_prev - distBetweenPts(x1, y1, x2, y2)) * CAMERA_ZOOM_DAMPER;
		
		update();
	}
	
	// This will prevent big skips on new touches.
	if ((phase1 == TouchPhaseMoved) || (phase2 == TouchPhaseMoved) || (phase1 == TouchPhaseBegan) || (phase2 == TouchPhaseBegan))
	{
		_skip_touch_multi = FALSE;
		_mult_touch_dist_prev = distBetweenPts(x1, y1, x2, y2);
	}
	
	if ((phase1 == TouchPhaseEnded) || (phase2 == TouchPhaseEnded))
	{
		_skip_touch_multi = TRUE;
		_mult_touch_dist_prev = 0;
	}
}


void CCamera::handleTouch(float x, float y, eTouchPhase phase)
{
	// Single touch will rotate the camera.
	if ((phase == TouchPhaseMoved) && (!_skip_touch))
	{
		_touch_dist.x += (_touch_pos_prev.x - x) * CAMERA_MOVE_DAMPER;
		_touch_dist.y += (_touch_pos_prev.y - y) * CAMERA_MOVE_DAMPER;
		
		update();
	}
	
	// This will prevent big skips on new touches.
	if ((phase == TouchPhaseMoved) || (phase == TouchPhaseBegan))
	{
		_skip_touch = FALSE;
		_touch_pos_prev.x = x;
		_touch_pos_prev.y = y;
	}
	
	if (phase == TouchPhaseEnded)
	{
		_skip_touch = TRUE;
		_touch_pos_prev = Vector3(0.0f, 0.0f, 0.0f);
	}
}