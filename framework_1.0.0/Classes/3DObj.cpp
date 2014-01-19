/*
 *  3DObj.cpp
 *  test_framework
 *
 *  Created by Johnny Chan on 2/10/10.
 *  Copyright 2010 LlamaFace. All rights reserved.
 *
 */


#include "3DObj.h"

C3DObj::C3DObj()
{
	
}

C3DObj::~C3DObj()
{
	CSprite::destroy();	
}

void C3DObj::init(float* verts, float* texCoords, float* normals, int numVerts)
{
	CSprite::init(0, 0);
	
	_verts = verts;
	_tex_coords = texCoords;
	_normals = normals;
	_num_verts = numVerts;
}