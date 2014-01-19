/*
 *  3DObj.h
 *  test_framework
 *
 *  Created by Johnny Chan on 2/10/10.
 *  Copyright 2010 LlamaFace. All rights reserved.
 *
 */

#include "Sprite.h"

class C3DObj : public CSprite
{
public:
	float* _verts;		/*!< The pointer to the array of vertices that this object contains. */
	float* _tex_coords;	/*!< The texture coordinates for this object. */
	float* _normals;	/*!< The normals of this object. */
	int	_num_verts;		/*!< The number of vertices that this object contains. */

	C3DObj();
	~C3DObj();
	
	void init(void);
	void init(float* verts, float* texCoords, float* normals, int numVerts);
};