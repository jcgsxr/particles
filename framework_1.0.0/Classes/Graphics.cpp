#include "Graphics.h"
#include "SystemDefines.h"
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "Math.h"
#include "MathUtil.h"
#include "types.h"
#include "physics_types.h"
#include "Utils.h"
#include <string.h>
#include "Engine.h"


#if defined (ENABLE_PNGLOAD)
const GLfloat tex_coords[] = 
{
	0.0, 0.0,	// Top left
	1.0, 0.0,	// Top right
	0.0, 1.0,	// Bottom left
	1.0, 1.0,	// Bottom right
};
#else
// Pixel data is loaded in a different order when using quartz than when png load is used.
// Texture coordinates when GL_TRIANGLES are used.
const GLfloat tex_tri_coords[] = 
{
	0.0, 1.0,	// Top left
	1.0, 1.0,	// Top right
	0.0, 0.0,	// Bottom left
	0.0, 0.0,	// Bottom left
	1.0, 1.0,	// Top right
	1.0, 0.0,	// Bottom right
};

// Texture coordinates when GL_TRIANGLE_STRIP is used.
const GLfloat tex_coords[] = 
{
	0.0, 1.0,	// Top left
	1.0, 1.0,	// Top right
	0.0, 0.0,	// Bottom left
	1.0, 0.0,	// Bottom right
};
#endif

const float normals_3D[] =
{
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
};



float CGraphics::getFloatColor(const int hexVal)
{
	return ((const float)hexVal / 255);
}

//0xRRGGBBAA
float CGraphics::getRCol(const int hexVal)
{
	return ((const float)((hexVal & 0xFF000000) >> 24) / 255);
}


float CGraphics::getGCol(const int hexVal)
{
	return ((const float)((hexVal & 0x00FF0000) >> 16) / 255);
}


float CGraphics::getBCol(const int hexVal)
{
	return ((const float)((hexVal & 0x0000FF00) >> 8) / 255);	
}


float CGraphics::getACol(const int hexVal)
{
	return ((const float)(hexVal & 0x000000FF) / 255);
}

void CGraphics::drawLine(POLine line, color lineColor)
{
	drawLine(line.start.x, line.start.y, line.end.x, line.end.y, lineColor);
}

void CGraphics::drawLine(const float x1, const float y1, const float x2, const float y2, color theColor)
{
	// If the alpha is zero, don't bother drawing.
	if (theColor.a <= 0.0)
	{
		return;
	}
	
	GLfloat vertices[4];

	glColor4f(theColor.r, theColor.g, theColor.b, theColor.a);
	
	vertices[0] = x1;
	vertices[1] = y1;
	vertices[2] = x2;
	vertices[3] = y2;
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	
	glDrawArrays(GL_LINES, 0, 2);
}


void CGraphics::drawTriangle(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, color theColor, BOOL bFilled)
{
	// If the alpha is zero, don't bother drawing.
	if (theColor.a <= 0.0)
	{
		return;
	}
	
	GLfloat vertices[6];

	glColor4f(theColor.r, theColor.g, theColor.b, theColor.a);
	
	vertices[0] = x1;
	vertices[1] = y1;
	vertices[2] = x2;
	vertices[3] = y2;
	vertices[4] = x3;
	vertices[5] = y3;
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(1);
#endif
}


void CGraphics::drawRect(const float x, const float y, const float w, const float h, color theColor, BOOL bFilled)
{
	// If the alpha is zero, don't bother drawing.
	if (theColor.a <= 0.0)
	{
		return;
	}
	
	GLfloat vertices[8];
	
	glColor4f(theColor.r, theColor.g, theColor.b, theColor.a);
	
	if (bFilled)
	{
		buildQuadVertices(vertices, x, y, w, h);
		
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		
		glDrawArrays(GL_QUADS, 0, 4);	
	}
	else
	{
		buildQuadLineLoopVertices(vertices, x, y, w, h);
		
		glVertexPointer(2, GL_FLOAT, 0, vertices);

		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}


void CGraphics::drawRect(colorRect colRect, BOOL bFilled)
{
	color theColor = {colRect.col.r, colRect.col.g, colRect.col.b, colRect.col.a};
	drawRect(colRect.x, colRect.y, colRect.w, colRect.h, theColor, bFilled);
}


void CGraphics::draw3DRect(const float x, const float y, const float z, const float w, const float h, color theColor, BOOL bFilled)
{
	// If the alpha is zero, don't bother drawing.
	if (theColor.a <= 0.0)
	{
		return;
	}
	
	GLfloat vertices[18];
	
	glColor4f(theColor.r, theColor.g, theColor.b, theColor.a);
	
	// Translate the coordinates to the 3d view coordinates.
	float trans_x, trans_y, trans_z;
	coordsScreenTo3D(x, y, z, &trans_x, &trans_y, &trans_z);
	
	if (bFilled)
	{
		buildTriangleVertices(vertices, trans_x, trans_y, trans_z, w / SCRN_W, h / SCRN_H);
		
		//glRotatef(180, 1.0, 0.0, 0.0);
		
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else
	{
		// JC: TODO: This probably doesn't work yet. Needs to be tested.
		buildQuadLineLoopVertices(vertices, x, y, w, h);
		
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}


void CGraphics::draw3DRect(colorRect colRect, BOOL bFilled)
{
	color theColor = {colRect.col.r, colRect.col.g, colRect.col.b, colRect.col.a};
	draw3DRect(colRect.x, colRect.y, colRect.z, colRect.w, colRect.h, theColor, bFilled);
}


void CGraphics::drawEllipse(GLfloat x, GLfloat y, GLfloat w, GLfloat h, const int segments, color theColor, BOOL bFilled)
{
	// If the alpha is zero, don't bother drawing.
	if (theColor.a <= 0.0)
	{
		return;
	}
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	
	GLfloat vertices[segments * 2];
	
	int count=0;
	
	for (GLfloat i = 0; i < 360.0f; i += (360.0f / segments))
	{
		vertices[count++] = (cos(DEGREES_TO_RADIANS(i)) * w);
		vertices[count++] = (sin(DEGREES_TO_RADIANS(i)) * h);
	}
	
	glColor4f(theColor.r, theColor.g, theColor.b, theColor.a);
	glVertexPointer (2, GL_FLOAT , 0, vertices); 
	
	if (bFilled)
	{
		glDrawArrays (GL_TRIANGLE_FAN, 0, segments);
	}
	else
	{
		glDrawArrays (GL_LINE_LOOP, 0, segments);
	}
	

	glPopMatrix();
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(segments);
#endif
}


void CGraphics::drawCircle(GLfloat x, GLfloat y, GLfloat radius, const int segments, color theColor, BOOL bFilled)
{
	drawEllipse(x, y, radius, radius, segments, theColor, bFilled);
}


void CGraphics::drawCircleSlice(GLfloat x, GLfloat y, GLfloat radius, const int segments, GLfloat startDeg, GLfloat endDeg, color theColor)
{
	// If the alpha is zero, don't bother drawing.
	if (theColor.a <= 0.0)
	{
		return;
	}
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	
	GLfloat vertices[segments * 2];
	
	int count = 0;
	
	// GL circles draw from the left and as the degrees increase, it draws clockwise.
	for (GLfloat i = 0.0; i < 360.0f; i += (360.0f / segments))
	{
		// Special case handling 360 to 0 vertex.
		if ((endDeg == 360.0) && (i == 0.0))
		{
			vertices[count++] = (cos(DEGREES_TO_RADIANS(i)) * radius);
			vertices[count++] = (sin(DEGREES_TO_RADIANS(i)) * radius);
		}
		else if ((i >= startDeg) && (i <= endDeg))
		{
			vertices[count++] = (cos(DEGREES_TO_RADIANS(i)) * radius);
			vertices[count++] = (sin(DEGREES_TO_RADIANS(i)) * radius);
		}
		else
		{
			vertices[count++] = 0;
			vertices[count++] = 0;
		}
	}
	
	glColor4f(theColor.r, theColor.g, theColor.b, theColor.a);
	glVertexPointer (2, GL_FLOAT , 0, vertices); 
	
	glDrawArrays (GL_TRIANGLE_FAN, 0, segments);
	
	glPopMatrix();
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(segments);
#endif
}

void CGraphics::drawRoundedRect(const float x, const float y, const float w, const float h, GLfloat cornerRadius, color theColor)
{
	// If the alpha is zero, don't bother drawing.
	if (theColor.a <= 0.0)
	{
		return;
	}
	
	// First draw the four corner circles.
	
	// Starting with top left.
	drawCircleSlice(
					x + cornerRadius, 
					y + cornerRadius, 
					cornerRadius, 
					ROUNDED_RECT_SEGMENTS, 
					180.0,
					270.0, 
					theColor);
	
	// Top Right.
	drawCircleSlice(
					x + w - cornerRadius, 
					y + cornerRadius, 
					cornerRadius, 
					ROUNDED_RECT_SEGMENTS, 
					270.0,
					360.0, 
					theColor);
	
	// Bottom left.
	drawCircleSlice(
					x + cornerRadius, 
					y + h - cornerRadius, 
					cornerRadius, 
					ROUNDED_RECT_SEGMENTS, 
					90.0,
					180.0, 
					theColor);
	
	// Bottom right.
	drawCircleSlice(
					x + w - cornerRadius, 
					y + h - cornerRadius, 
					cornerRadius, 
					ROUNDED_RECT_SEGMENTS, 
					0.0,
					90.0, 
					theColor);
	
	// JC: This can and should be optimized more.
	// Now draw the rects to fill out the middle.
	// Top strip.
	drawRect(
			 x + cornerRadius, 
			 y, 
			 w - (cornerRadius * 2), 
			 cornerRadius, 
			 theColor);
	// Left strip.
	drawRect(
			 x, 
			 y + cornerRadius,
			 cornerRadius, 
			 h - (cornerRadius * 2), 
			 theColor);
	// Right strip.
	drawRect(
			 x + w - cornerRadius, 
			 y + cornerRadius, 
			 cornerRadius, 
			 h - (cornerRadius * 2), 
			 theColor);
	// Bottom strip.
	drawRect(
			 x + cornerRadius,
			 y + h - cornerRadius,
			 w - (cornerRadius * 2),
			 cornerRadius,
			 theColor);
	
	// Finally, draw the middle rect.
	drawRect(
			 x + cornerRadius, 
			 y + cornerRadius,
			 w - (cornerRadius * 2),
			 h - (cornerRadius * 2),
			 theColor);
}

// Since we want to support transparent rounded rects, we do a little extra work to make it look good.
void CGraphics::drawRoundedRect(const float x, const float y, const float w, const float h, color theColor)
{
	drawRoundedRect(x, y, w, h, ROUNDED_RECT_CORNER_RADIUS, theColor);
}

void CGraphics::drawRoundedRect(colorRect pRect)
{
	color theColor = {pRect.col.r, pRect.col.g, pRect.col.b, pRect.col.a};
	drawRoundedRect(pRect.x, pRect.y, pRect.w, pRect.h, theColor);
}


void CGraphics::draw3DObj(C3DObj* obj)
{
	draw3DObj(obj, obj->_x, obj->_y, obj->_z);
}

void CGraphics::draw3DObj(C3DObj* obj, const float x, const float y, const float z)
{
	if (obj == NULL)
	{
		DPRINT_GRAPHICS("CGraphics::draw3DObj failed: obj is null\n");
		return;
	}
	
	CImage* image = obj->getImage();
	
	if ((image == NULL) || (!obj->isVisible()))
	{
		DPRINT_GRAPHICS("CGraphics::draw3DObj failed: image is null or object is set to not visible\n");
		return;
	}
	
	// Translate the coordinates to the 3d view coordinates.
	float trans_x, trans_y, trans_z;
	coordsScreenTo3D(x, y, z, &trans_x, &trans_y, &trans_z);

	glPushMatrix();
	//glLoadIdentity();
	
	// Make sure to enable the states that let us bind and draw the texture.
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, obj->getTexName());
	glVertexPointer(3, GL_FLOAT, 0, obj->_verts);
	glNormalPointer(GL_FLOAT, 0, obj->_normals);
	glTexCoordPointer(2, GL_FLOAT, 0, obj->_tex_coords);
	
	// Scale object if needed.
	// Skip scaling if the scale is 1 to 1.
	if ((obj->_scale.x != 1.0) || (obj->_scale.y != 1.0) || (obj->_scale.z != 1.0))
	{
		// Set the scale.
		glScalef(obj->_scale.x, obj->_scale.y, obj->_scale.z);
	}
	// Move object to the given coords.
	glTranslatef(trans_x, trans_y, trans_z);
	// Execute rotation last, if any. This ensures that the object will be rotated relative to it's current position.
	glRotatef(obj->_angle.x, 1.0, 0.0, 0.0);
	glRotatef(obj->_angle.y, 0.0, 1.0, 0.0);
	glRotatef(obj->_angle.z, 0.0, 0.0, 1.0);
	
	// Set colors back to white.
	glColor4f(1, 1, 1, obj->_color.a);
    glDrawArrays(GL_TRIANGLES, 0, obj->_num_verts);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(obj->_num_verts);
#endif
}


void CGraphics::draw3DSpriteCentered(const CSprite* sprite)
{
	draw3DSprite(
				 sprite, 
				 sprite->_x - (sprite->getHalfWidth() * sprite->_scale.x), 
				 sprite->_y - (sprite->getHalfHeight() * sprite->_scale.y),
				 sprite->_z);
				 //sprite->_z - (sprite->getHalfDepth() * sprite->_scale.z));
}


void CGraphics::draw3DSpriteCentered(const CSprite* sprite, const float x, const float y, const float z)
{
//	draw3DSprite(
//				 sprite, 
//				 x - (sprite->getHalfWidth() * sprite->_scale.x), 
//				 y - (sprite->getHalfHeight() * sprite->_scale.y), 
//				 z);
	draw3DSprite(
				 sprite, 
				 x - ((sprite->getHalfWidth() / 2) * sprite->_scale.x), 
				 y + ((sprite->getHalfHeight() / 2) * sprite->_scale.y), 
				 z);
}


void CGraphics::draw3DSpriteCenteredLookAt(const CSprite* sprite, const float* camMat)
{
	draw3DSpriteLookAt(
					   sprite, 
					   sprite->_x - (sprite->getHalfWidth() * sprite->_scale.x), 
					   sprite->_y - (sprite->getHalfHeight() * sprite->_scale.y),
					   sprite->_z,
					   camMat);
}


void CGraphics::draw3DSpriteCenteredLookAt(const CSprite* sprite, const float x, const float y, const float z, const float* camMat)
{
	draw3DSpriteLookAt(
					   sprite, 
					   x - ((sprite->getHalfWidth() / 2) * sprite->_scale.x), 
					   y + ((sprite->getHalfHeight() / 2) * sprite->_scale.y), 
					   z,
					   camMat);
}

void CGraphics::draw3DSprite(const CSprite* sprite)
{	
	draw3DSprite(sprite, sprite->_x, sprite->_y, sprite->_z);
}


void CGraphics::draw3DSpriteLookAt(const CSprite* sprite, const float x, const float y, const float z, const float* camMat)
{
	if (sprite == NULL)
	{
		return;
	}
	
	CImage* image = sprite->getImage();
	
	if ((image == NULL) || (!sprite->isVisible()))
	{
		return;
	}
		
	// If the alpha is zero, don't bother drawing.
	if (sprite->_color.a <= 0.0)
	{
		return;
	}
	
	float trans_x, trans_y, trans_z;
	float halfwidth, halfheight, halfdepth;
	float trans_w, trans_h, trans_size;;
	float trans_center_x, trans_center_y;
	GLfloat vertices[12];
	memset(&vertices, 0, sizeof(GLfloat) * 12);
	
	// Translate the coordinates to the 3d view coordinates.
	coordsScreenTo3D(x, y, z, &trans_x, &trans_y, &trans_z);
	
	// Translate the dimensions to the 3d view coordinates.
//	halfwidth = (const float)(sprite->getHalfWidth()) / SCRN_W;
//	halfheight = (const float)(sprite->getHalfHeight()) / SCRN_H;
//	halfdepth = (const float)(sprite->getHalfDepth()) / SCRN_D;
	halfwidth = ((const float)(sprite->getHalfWidth()) / SCRN_W) / 2;
	halfheight = ((const float)(sprite->getHalfHeight()) / SCRN_H) / 2;
	halfdepth = ((const float)(sprite->getHalfDepth()) / SCRN_D) / 2;
	
	trans_center_x = trans_x + halfwidth;
	trans_center_y = trans_y + halfheight;
	trans_w = (const float)sprite->getWidth() / SCRN_W;
	trans_h = (const float)sprite->getHeight() / SCRN_H;
	trans_size = trans_w;
	
	//buildQuadVerticesCentered(vertices, trans_center_x, trans_center_y, trans_z, (const float)image->getWidth() / SCRN_W, (const float)image->getHeight() / SCRN_H);
	
	// Position the sprite to always front-face the camera/viewer.
	float right[3], up[3];
	right[0] = camMat[0];
	right[1] = camMat[4];
	right[2] = camMat[8];
	up[0] = camMat[1];
	up[1] = camMat[5];
	up[2] = camMat[9];
	
	vertices[0] = trans_center_x + ((-right[0] - up[0]) * trans_size);
	vertices[1] = trans_center_y + ((-right[1] - up[1]) * trans_size);
	vertices[2] = trans_z + ((-right[2] - up[2]) * trans_size);
	
	vertices[3] = trans_center_x + ((right[0] - up[0]) * trans_size);
	vertices[4] = trans_center_y + ((right[1] - up[1]) * trans_size);
	vertices[5] = trans_z + ((right[2] - up[2]) * trans_size);
	
	vertices[6] = trans_center_x + ((-right[0] + up[0]) * trans_size);
	vertices[7] = trans_center_y + ((-right[1] + up[1]) * trans_size);
	vertices[8] = trans_z + ((-right[2] + up[2]) * trans_size);
	
	vertices[9] = trans_center_x + ((right[0] + up[0]) * trans_size);
	vertices[10] = trans_center_y + ((right[1] + up[1]) * trans_size);
	vertices[11] = trans_z + ((right[2] + up[2]) * trans_size);
	
	
	glPushMatrix();
	
	// Since we want to rotate at the middle point of the sprite, we need to translate it.
	glTranslatef(
				 trans_x + (halfwidth * sprite->_scale.x), 
				 trans_y + (halfheight * sprite->_scale.y), 
				 trans_z + (halfdepth * sprite->_scale.z));
	
	// Execute rotation, if any.
	glRotatef(sprite->_angle.x, 1.0, 0.0, 0.0);
	glRotatef(sprite->_angle.y, 0.0, 1.0, 0.0);
	glRotatef(sprite->_angle.z, 0.0, 0.0, 1.0);
	
	// Translate back to the original position.
	glTranslatef(
				 -trans_x - (halfwidth * sprite->_scale.x), 
				 -trans_y - (halfheight * sprite->_scale.y), 
				 -trans_z - (halfdepth * sprite->_scale.z));
	
	// Skip scaling if the scale is 1 to 1.
	if ((sprite->_scale.x != 1.0) || (sprite->_scale.y != 1.0) || (sprite->_scale.z != 1.0))
	{
		// We need to translate to the proper position based on the scale since scaling affects the coords.
		glTranslatef(
					 trans_x - (trans_x * sprite->_scale.x), 
					 trans_y - (trans_y * sprite->_scale.y), 
					 trans_z - (trans_z * sprite->_scale.z));
		
		// Set the scale.
		glScalef(sprite->_scale.x, sprite->_scale.y, sprite->_scale.z);
	}

	// Set colors.
	glColor4f(sprite->_color.r, sprite->_color.g, sprite->_color.b, sprite->_color.a);

	// Make sure to enable the states that let us bind and draw the texture.
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, sprite->getTexName());
	
	//	glVertexPointer(3, GL_FLOAT, 0, vertices);
	//	glTexCoordPointer(2, GL_FLOAT, 0, tex_tri_coords);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}



void CGraphics::draw3DSprite(const CSprite* sprite, const float x, const float y, const float z)
{
	if (sprite == NULL)
	{
		return;
	}
	
	CImage* image = sprite->getImage();
	
	if ((image == NULL) || (!sprite->isVisible()))
	{
		return;
	}
	
	// If the alpha is zero, don't bother drawing.
	if (sprite->_color.a <= 0.0)
	{
		return;
	}
	
	float trans_x, trans_y, trans_z;
	float halfwidth, halfheight, halfdepth;
	//GLfloat vertices[18];
	GLfloat vertices[12];
	memset(&vertices, 0, sizeof(GLfloat) * 12);
	
	// Translate the coordinates to the 3d view coordinates.
	coordsScreenTo3D(x, y, z, &trans_x, &trans_y, &trans_z);
	
	// Translate the dimensions to the 3d view coordinates.
	halfwidth = (const float)(sprite->getHalfWidth()) / SCRN_W;
	halfheight = (const float)(sprite->getHalfHeight()) / SCRN_H;
	halfdepth = (const float)(sprite->getHalfDepth()) / SCRN_D;
	
	//buildTriangleVertices(vertices, trans_x, trans_y, trans_z, (const float)image->getWidth() / SCRN_W, (const float)image->getHeight() / SCRN_H);
	buildQuadVertices(vertices, trans_x, trans_y, trans_z, (const float)image->getWidth() / SCRN_W, (const float)image->getHeight() / SCRN_H);
	
	glPushMatrix();
	
	// Since we want to rotate at the middle point of the sprite, we need to translate it.
	glTranslatef(
				 trans_x + (halfwidth * sprite->_scale.x), 
				 trans_y + (halfheight * sprite->_scale.y), 
				 trans_z + (halfdepth * sprite->_scale.z));
	
	// Execute rotation, if any.
	glRotatef(sprite->_angle.x, 1.0, 0.0, 0.0);
	glRotatef(sprite->_angle.y, 0.0, 1.0, 0.0);
	glRotatef(sprite->_angle.z, 0.0, 0.0, 1.0);
	
	// Translate back to the original position.
	glTranslatef(
				 -trans_x - (halfwidth * sprite->_scale.x), 
				 -trans_y - (halfheight * sprite->_scale.y), 
				 -trans_z - (halfdepth * sprite->_scale.z));
	
	// Skip scaling if the scale is 1 to 1.
	if ((sprite->_scale.x != 1.0) || (sprite->_scale.y != 1.0) || (sprite->_scale.z != 1.0))
	{
		// We need to translate to the proper position based on the scale since scaling affects the coords.
		glTranslatef(
					 trans_x - (trans_x * sprite->_scale.x), 
					 trans_y - (trans_y * sprite->_scale.y), 
					 trans_z - (trans_z * sprite->_scale.z));
		
		// Set the scale.
		glScalef(sprite->_scale.x, sprite->_scale.y, sprite->_scale.z);
	}
	
	// Set colors.
	glColor4f(sprite->_color.r, sprite->_color.g, sprite->_color.b, sprite->_color.a);
	
	// Make sure to enable the states that let us bind and draw the texture.
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, sprite->getTexName());
	
//	glVertexPointer(3, GL_FLOAT, 0, vertices);
//	glTexCoordPointer(2, GL_FLOAT, 0, tex_tri_coords);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}


void CGraphics::draw3DSprite(const CSprite* sprite, const float x, const float y, const float z, const float offsetX, const float offsetY, const float clipW, const float clipH)
{
	if (sprite == NULL)
	{
		return;
	}
	
	CImage* image = sprite->getImage();
	
	if ((image == NULL) || (!sprite->isVisible()))
	{
		return;
	}
	
	// If the alpha is zero, don't bother drawing.
	if (sprite->_color.a <= 0.0)
	{
		return;
	}
	
	float trans_x, trans_y, trans_z;
	float halfwidth, halfheight, halfdepth;
	GLfloat vertices[18];
	//GLfloat vertices[12];
	GLfloat texCoords[12];
	
	// Translate the coordinates to the 3d view coordinates.
	coordsScreenTo3D(x, y, z, &trans_x, &trans_y, &trans_z);
	
	// Translate the dimensions to the 3d view coordinates.
	halfwidth = (const float)(sprite->getHalfWidth()) / SCRN_W;
	halfheight = (const float)(sprite->getHalfHeight()) / SCRN_H;
	halfdepth = (const float)(sprite->getHalfDepth()) / SCRN_D;
	
	// Construct the vertices using the translated coordinates.
	buildTriangleVertices(vertices, trans_x, trans_y, trans_z, (const float)clipW / SCRN_W, (const float)clipH / SCRN_H);
	//buildQuadVertices(vertices, trans_x, trans_y, trans_z, (const float)image->getWidth() / SCRN_W, (const float)image->getHeight() / SCRN_H);
	
	// Construct the texture coordinates. These will effectively clip from the given image.
	buildTriangleTexCoords(texCoords, offsetX, offsetY, clipW, clipH, image);
	
	
	glPushMatrix();
	
	// Since we want to rotate at the middle point of the sprite, we need to translate it.
	glTranslatef(
				 trans_x + (halfwidth * sprite->_scale.x), 
				 trans_y + (halfheight * sprite->_scale.y), 
				 trans_z + (halfdepth * sprite->_scale.z));
	
	// Execute rotation, if any.
	glRotatef(sprite->_angle.x, 1.0, 0.0, 0.0);
	glRotatef(sprite->_angle.y, 0.0, 1.0, 0.0);
	glRotatef(sprite->_angle.z, 0.0, 0.0, 1.0);
	
	// Translate back to the original position.
	glTranslatef(
				 -trans_x - (halfwidth * sprite->_scale.x), 
				 -trans_y - (halfheight * sprite->_scale.y), 
				 -trans_z - (halfdepth * sprite->_scale.z));
	
	// Skip scaling if the scale is 1 to 1.
	if ((sprite->_scale.x != 1.0) || (sprite->_scale.y != 1.0) || (sprite->_scale.z != 1.0))
	{
		// We need to translate to the proper position based on the scale since scaling affects the coords.
		glTranslatef(
					 trans_x - (trans_x * sprite->_scale.x), 
					 trans_y - (trans_y * sprite->_scale.y), 
					 trans_z - (trans_z * sprite->_scale.z));
		
		// Set the scale.
		glScalef(sprite->_scale.x, sprite->_scale.y, sprite->_scale.z);
	}
	
	// Set colors.
	glColor4f(sprite->_color.r, sprite->_color.g, sprite->_color.b, sprite->_color.a);
	
	// Make sure to enable the states that let us bind and draw the texture.
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, sprite->getTexName());
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	//glNormalPointer(GL_FLOAT, 0, normals_3D);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	//glVertexPointer(3, GL_FLOAT, 0, vertices);
	//glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}


void CGraphics::drawSprite(const CSprite* sprite, const float x, const float y)
{	
	if (sprite == NULL)
	{
		return;
	}
	
	CImage* image = sprite->getImage();
	
	if ((image == NULL) || (!sprite->isVisible()) || (sprite->_color.a <= 0.0))
	{
		return;
	}
	
	// If the alpha is zero, don't bother drawing.
	if (sprite->_color.a <= 0.0)
	{
		return;
	}

#if ENABLE_TRIANGLE_QUADS
	// A quad consists of 2 triangles, so we need to create 6 vertices for it.
	GLfloat vertices[12];
	buildTriangleVertices(vertices, x, y, image->getWidth(), image->getHeight());
#else
	// 4 vertices for a regular quad.
	GLfloat vertices[8];
	buildQuadVertices(vertices, x, y, image->getWidth(), image->getHeight());
#endif
	
	glPushMatrix();
	//glLoadIdentity();
	
	// Since we want to rotate at the middle point of the sprite, we need to translate it.
	glTranslatef(
				 x + (sprite->getHalfWidth() * sprite->_scale.x), 
				 y + (sprite->getHalfHeight() * sprite->_scale.y), 
				 0);
	
	// Execute rotation, if any.
	glRotatef(sprite->_angle.x, 1.0, 0.0, 0.0);
	glRotatef(sprite->_angle.y, 0.0, 1.0, 0.0);
	glRotatef(sprite->_angle.z, 0.0, 0.0, 1.0);
	
	// Translate back to the original position.
	glTranslatef(
				 -x - (sprite->getHalfWidth() * sprite->_scale.x), 
				 -y - (sprite->getHalfHeight() * sprite->_scale.y), 
				 0);
	
	// Skip scaling if the scale is 1 to 1.
	if ((sprite->_scale.x != 1.0) || (sprite->_scale.y != 1.0) || (sprite->_scale.z != 1.0))
	{
		// We need to translate to the proper position based on the scale since scaling affects the coords.
		glTranslatef(
					 x - (x * sprite->_scale.x), 
					 y - (y * sprite->_scale.y), 
					 0);
				
		// Set the scale.
		glScalef(sprite->_scale.x, sprite->_scale.y, sprite->_scale.z);
	}
	
	// Set colors.
	glColor4f(sprite->_color.r, sprite->_color.g, sprite->_color.b, sprite->_color.a);
	
	// Make sure to enable the states that let us bind and draw the texture.
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, sprite->getTexName());
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	
#if ENABLE_TRIANGLE_QUADS
	// Rendering triangle requires a different set of texture coordinates.
	glTexCoordPointer(2, GL_FLOAT, 0, tex_tri_coords);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#else
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#endif
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}


void CGraphics::drawSpriteCentered(const CSprite &sprite)
{
	drawSprite(&sprite, sprite._x - (sprite.getHalfWidth() * sprite._scale.x), sprite._y - (sprite.getHalfHeight() * sprite._scale.y));
}
void CGraphics::drawSpriteCentered(const CSprite &sprite, const float x, const float y)
{
	drawSprite(&sprite, x - (sprite.getHalfWidth() * sprite._scale.x), y - (sprite.getHalfHeight() * sprite._scale.y));	
}
void CGraphics::drawSprite(const CSprite &sprite)
{	
	drawSprite(&sprite, sprite._x, sprite._y);
}
void CGraphics::drawSprite(const CSprite &sprite, const float x, const float y)
{	
	drawSprite(&sprite, x, y);
}
void CGraphics::drawSprite(const CSprite &sprite, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH)
{
	drawSprite(&sprite, x, y, offsetX, offsetY, clipW, clipH);
}
void CGraphics::draw3DSprite(const CSprite& sprite)
{	
	draw3DSprite(&sprite, sprite._x, sprite._y, sprite._z);
}
void CGraphics::draw3DSprite(const CSprite& sprite, const float x, const float y, const float z)
{	
	draw3DSprite(&sprite, x, y, z);
}
void CGraphics::draw3DSprite(const CSprite& sprite, const float x, const float y, const float z, const float offsetX, const float offsetY, const float clipW, const float clipH)
{
	draw3DSprite(&sprite, x, y, z, offsetX, offsetY, clipW, clipH);
}
void CGraphics::draw3DSpriteCenteredLookAt(const CSprite& sprite, const float* camMat)
{
	draw3DSpriteCenteredLookAt(&sprite, camMat);
}
void CGraphics::draw3DSpriteCenteredLookAt(const CSprite& sprite, const float x, const float y, const float z, const float* camMat)
{
	draw3DSpriteCenteredLookAt(&sprite, x, y, z, camMat);
}



void CGraphics::drawSpriteCentered(const CSprite* sprite)
{
	drawSprite(sprite, sprite->_x - (sprite->getHalfWidth() * sprite->_scale.x), sprite->_y - (sprite->getHalfHeight() * sprite->_scale.y));
}


void CGraphics::drawSpriteCentered(const CSprite* sprite, const float x, const float y)
{
	drawSprite(sprite, x - (sprite->getHalfWidth() * sprite->_scale.x), y - (sprite->getHalfHeight() * sprite->_scale.y));	
}


void CGraphics::drawSprite(const CSprite* sprite)
{	
	drawSprite(sprite, sprite->_x, sprite->_y);
}


void CGraphics::drawSprite(const CSprite* sprite, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH)
{	
	if (sprite == NULL)
	{
		return;
	}
	
	CImage* image = sprite->getImage();
	
	if ((image == NULL) || (!sprite->isVisible()))
	{
		return;
	}
	
	// If the alpha is zero, don't bother drawing.
	if (sprite->_color.a <= 0.0)
	{
		return;
	}
	
	GLfloat vertices[8];
	GLfloat texCoords[8];
	
	buildQuadVertices(vertices, x, y, clipW, clipH);
	buildQuadTexCoords(texCoords, offsetX, offsetY, clipW, clipH, image);
	
	glPushMatrix();
	
	// Since we want to rotate at the middle point of the sprite, we need to translate it.
	glTranslatef(
				 x + (sprite->getHalfWidth() * sprite->_scale.x), 
				 y + (sprite->getHalfHeight() * sprite->_scale.y), 
				 0);
	
	// Execute rotation, if any.
	glRotatef(sprite->_angle.x, 1.0, 0.0, 0.0);
	glRotatef(sprite->_angle.y, 0.0, 1.0, 0.0);
	glRotatef(sprite->_angle.z, 0.0, 0.0, 1.0);
	
	// Translate back to the original position.
	glTranslatef(
				 -x - (sprite->getHalfWidth() * sprite->_scale.x), 
				 -y - (sprite->getHalfHeight() * sprite->_scale.y), 
				 0);
	
	// Skip scaling if the scale is 1 to 1.
	if ((sprite->_scale.x != 1.0) || (sprite->_scale.y != 1.0) || (sprite->_scale.z != 1.0))
	{
		// We need to translate to the proper position based on the scale since scaling affects the coords.
		glTranslatef(
					 x - (x * sprite->_scale.x), 
					 y - (y * sprite->_scale.y), 
					 0);
		
		// Set the scale.
		glScalef(sprite->_scale.x, sprite->_scale.y, sprite->_scale.z);
	}
	
	// Set colors.
	glColor4f(sprite->_color.r, sprite->_color.g, sprite->_color.b, sprite->_color.a);
	
	// Make sure to enable the states that let us bind and draw the texture.
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, sprite->getTexName());
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}


void CGraphics::bindImage(const CImage* image)
{
	GLuint texture_name;
	
	glGenTextures (1, &texture_name);
	glBindTexture (GL_TEXTURE_2D, texture_name);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
	
	glTexImage2D (GL_TEXTURE_2D, 
				  0, 
				  image->getGLFormat(),
				  image->getWidth(),
				  image->getHeight(),
				  0, 
				  image->getGLFormat(), 
				  GL_UNSIGNED_BYTE,
				  image->getImageData());
}


void CGraphics::drawImage(const CImage* image, const float x, const float y, GLuint texName, const float alpha)
{	
	if (image == NULL)
	{
		return;
	}
	
	if (alpha <= 0.0f)
	{
		return;
	}
	
	glBindTexture (GL_TEXTURE_2D, texName);
	
	GLfloat vertices[8];
	
	buildQuadVertices(vertices, x, y, image->getWidth(), image->getHeight());
	
	// Set colors back to white.
	glColor4f(1, 1, 1, alpha);
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}


void CGraphics::drawImage(const CImage* image, const float x, const float y, GLuint texName)
{	
	if (image == NULL)
	{
		return;
	}
	
	glBindTexture (GL_TEXTURE_2D, texName);
	
	GLfloat vertices[8];
	
	buildQuadVertices(vertices, x, y, image->getWidth(), image->getHeight());
	
	// Set colors back to white.
	glColor4f(1, 1, 1, 1);
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}

void CGraphics::drawImage(const CImage* image, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH)
{
	if (image == NULL)
	{
		return;
	}
	
	GLfloat vertices[8];
	GLfloat texCoords[8];
	
	buildQuadVertices(vertices, x, y, clipW, clipH);
	buildQuadTexCoords(texCoords, offsetX, offsetY, clipW, clipH, image);
	
	// Set colors back to white.
	glColor4f(1, 1, 1, 1);
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	//glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
#if defined (ENABLE_POLY_COUNT)
	updatePolyCount(2);
#endif
}

// Builds a quad using 2 triangles.
void CGraphics::buildTriangleVertices(GLfloat* vertices, GLfloat x, GLfloat y , GLfloat w, GLfloat h)
{
	// Build first triangle.
	// Top left.
	vertices[0] = x;
	vertices[1] = y + h;
	// Top right;
	vertices[2] = x + w;
	vertices[3] = y + h;
	// Bottom left.
	vertices[4] = x;
	vertices[5] = y;
	
	// Build second triangle.
	// Bottom left.
	vertices[6] = x;
	vertices[7] = y;
	// Top right.
	vertices[8] = x + w;
	vertices[9] = y + h;
	// Bottom right.
	vertices[10] = x + w;
	vertices[11] = y;
}


// Builds a quad using 2 triangles.
void CGraphics::buildTriangleVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h)
{
	// Build first triangle.
	// Top left.
	vertices[0] = x;
	vertices[1] = y + h;
	vertices[2] = z;
	// Top right;
	vertices[3] = x + w;
	vertices[4] = y + h;
	vertices[5] = z;
	// Bottom left.
	vertices[6] = x;
	vertices[7] = y;
	vertices[8] = z;
	
	// Build second triangle.
	// Bottom left.
	vertices[9] = x;
	vertices[10] = y;
	vertices[11] = z;
	// Top right.
	vertices[12] = x + w;
	vertices[13] = y + h;
	vertices[14] = z;
	// Bottom right.
	vertices[15] = x + w;
	vertices[16] = y;
	vertices[17] = z;
}


// Builds the vertices in such a way that a rect frame will draw correctly.
void CGraphics::buildQuadLineLoopVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	// Top left.
	vertices[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x);		//	X
	vertices[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y + h);	//	Y
	
	// Top right.
	vertices[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x + w);	//	X
	vertices[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y + h);	//	Y

	// Bottom right.
	vertices[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x + w);	//	X
	vertices[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y);		//	Y
	
	// Bottom left.
	vertices[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x);	//	X
	vertices[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y);	//	Y
}


// Build standard quad vertices.
void CGraphics::buildQuadVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
	// Top left.
	vertices[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x);		//	X
	vertices[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y + h);	//	Y
	
	// Top right.
	vertices[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x + w);	//	X
	vertices[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y + h);	//	Y
	
	// Bottom left.
	vertices[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x);	//	X
	vertices[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y);	//	Y
	
	// Bottom right.
	vertices[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (x + w);	//	X
	vertices[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (y);		//	Y
}


void CGraphics::buildQuadVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h)
{
	// Top left.
	vertices[(0 * NUM_ELEMENTS_PER_VERTEX_3D) + 0] = (x);		//	X
	vertices[(0 * NUM_ELEMENTS_PER_VERTEX_3D) + 1] = (y);		//	Y
	vertices[(0 * NUM_ELEMENTS_PER_VERTEX_3D) + 2] = (z);		//	Z
	
	// Top right.
	vertices[(1 * NUM_ELEMENTS_PER_VERTEX_3D) + 0] = (x + w);	//	X
	vertices[(1 * NUM_ELEMENTS_PER_VERTEX_3D) + 1] = (y);		//	Y
	vertices[(1 * NUM_ELEMENTS_PER_VERTEX_3D) + 2] = (z);		//	Z
	
	// Bottom left.
	vertices[(2 * NUM_ELEMENTS_PER_VERTEX_3D) + 0] = (x);		//	X
	vertices[(2 * NUM_ELEMENTS_PER_VERTEX_3D) + 1] = (y + h);	//	Y
	vertices[(2 * NUM_ELEMENTS_PER_VERTEX_3D) + 2] = (z);		//	Z	
	
	// Bottom right.
	vertices[(3 * NUM_ELEMENTS_PER_VERTEX_3D) + 0] = (x + w);	//	X
	vertices[(3 * NUM_ELEMENTS_PER_VERTEX_3D) + 1] = (y + h);	//	Y
	vertices[(3 * NUM_ELEMENTS_PER_VERTEX_3D) + 2] = (z);		//	Z
}


void CGraphics::buildQuadTexCoords(GLfloat* texCoords, const float x, const float y, const float w, const float h, const CImage* image)
{
	float offsetX, offsetY, clipW, clipH;
	
#if defined (ENABLE_PNGLOAD)
	offsetX = (x  / image->getWidth());
	offsetY = (image->getHeight() - (y + h))/ image->getHeight();
	clipW = (w / image->getWidth());
	clipH = (h / image->getHeight());	
	
	// Top left.
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);	//	X
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);	//	Y

	// Top right.
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);			//	Y
	
	// Bottom left.
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);			//	X
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
	
	// Bottom right.
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
#else
	offsetX = (x  / image->getWidth());
	offsetY = (y / image->getHeight());
	clipW = (w / image->getWidth());
	clipH = (h / image->getHeight());	
	
	// Top left.
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);	//	X
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
	
	// Top right.
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);			//	Y
	
	// Bottom left.
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);			//	X
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);	//	Y
	
	// Bottom right.
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);	//	Y
#endif
}


void CGraphics::buildTriangleTexCoords(GLfloat* texCoords, const float x, const float y, const float w, const float h, const CImage* image)
{
	float offsetX, offsetY, clipW, clipH;
	
#if defined (ENABLE_PNGLOAD)
	offsetX = (x  / image->getWidth());
	offsetY = (image->getHeight() - (y + h))/ image->getHeight();

	clipW = (w / image->getWidth());
	clipH = (h / image->getHeight());
	
	// Top left.
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);	//	X
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);	//	Y
	
	// Top right.
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);			//	Y
	
	// Bottom left.
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);			//	X
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y

	// Bottom left.
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);			//	X
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
	
	// Top right.
	texCoords[(4 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(4 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);			//	Y
	
	// Bottom right.
	texCoords[(5 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(5 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
#else
	offsetX = (x  / image->getWidth());
	offsetY = (y / image->getHeight());
	clipW = (w / image->getWidth());
	clipH = (h / image->getHeight());	
	
	// Top left.
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);			//	X
	texCoords[(0 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
	
	// Top right.
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(1 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
	
	// Bottom left.
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);	//	X
	texCoords[(2 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);	//	Y
	
	// Bottom left.
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX);	//	X
	texCoords[(3 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);	//	Y
	
	// Top right.
	texCoords[(4 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(4 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY + clipH);	//	Y
	
	// Bottom right.
	texCoords[(5 * NUM_ELEMENTS_PER_VERTEX_2D) + 0] = (offsetX + clipW);	//	X
	texCoords[(5 * NUM_ELEMENTS_PER_VERTEX_2D) + 1] = (offsetY);			//	Y
#endif
}