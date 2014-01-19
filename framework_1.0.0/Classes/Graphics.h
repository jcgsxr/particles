#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "types.h"
#include "physics_types.h"
#include "Image.h"
#include "Sprite.h"
#include "3DObj.h"
#include "Utils.h"


#define NUM_ELEMENTS_PER_VERTEX_2D	2
#define NUM_ELEMENTS_PER_VERTEX_3D	3

static const GLfloat ROUNDED_RECT_CORNER_RADIUS = 5.0;
static const int ROUNDED_RECT_SEGMENTS = 8;
static const color black_color = {0, 0, 0, 1.0};

/*! \class CGraphics
 * \brief The Graphics class.
 *
 * The Graphics class contains rendering-related methods. As a utility class, all graphics methods are accessible globally.
 */
class CGraphics
{
public:	
	/*! \fn getFloatColor(const int hexVal)
	 *  \brief Given a color in the range of 0-255, returns the color as a float value in the range of 0.0-1.0
	 *  
	 *	\param hexVal The value of the color to convert to a float.
	 *  \return n/a
	 */
	static float getFloatColor(const int hexVal);
	
	/*! \fn getRCol(const int hexVal)
	 *  \brief Given a color in the format of 0xRRGGBBAA, returns the red color as a float value in the range of 0.0-1.0
	 *  
	 *	\param hexVal The value of the color to convert to a float.
	 *  \return n/a
	 */
	static float getRCol(const int hexVal);
	
	/*! \fn getGCol(const int hexVal)
	 *  \brief Given a color in the format of 0xRRGGBBAA, returns the green color as a float value in the range of 0.0-1.0
	 *  
	 *	\param hexVal The value of the color to convert to a float.
	 *  \return n/a
	 */
	static float getGCol(const int hexVal);
	
	/*! \fn getBCol(const int hexVal)
	 *  \brief Given a color in the format of 0xRRGGBBAA, returns the blue color as a float value in the range of 0.0-1.0
	 *  
	 *	\param hexVal The value of the color to convert to a float.
	 *  \return n/a
	 */
	static float getBCol(const int hexVal);
	
	/*! \fn getACol(const int hexVal)
	 *  \brief Given a color in the format of 0xRRGGBBAA, returns the alpha value as a float value in the range of 0.0-1.0
	 *  
	 *	\param hexVal The value of the color to convert to a float.
	 *  \return n/a
	 */
	static float getACol(const int hexVal);

	/*! \fn draw3DObj(C3DObj* obj, const float x, const float y, const float z)
	 *  \brief Renders a 3D object on the screen.
	 *  
	 *	\param obj The 3D object to be rendered.
	 *	\param x The x location where the sprite will be rendered. This is automatically translated to 3D view coordinates.
	 *	\param y The y location where the sprite will be rendered. This is automatically translated to 3D view coordinates.
 	 *	\param z The z location where the sprite will be rendered. This is automatically translated to 3D view coordinates.
	 *  \return n/a
	 */
	static void draw3DObj(C3DObj* obj, const float x, const float y, const float z);
	
	/*! \fn draw3DObj(C3DObj* obj)
	 *  \brief Renders a 3D object on the screen.
	 *  
	 *	\param obj The 3D object to be rendered.
	 *  \return n/a
	 */
	static void draw3DObj(C3DObj* obj);
	
	/*! \fn drawLine(POLine line, color theColor = black_color)
	 *  \brief Given a line physics object, renders it to the screen.
	 *  
	 *	\param line The line physics object.
	 *	\param theColor The color of the line.
	 *  \return n/a
	 */
	static void drawLine(POLine line, color theColor = black_color);
	
	/*! \fn drawLine(const float x1, const float y1, const float x2, const float y2, color theColor)
	 *  \brief Given a two point coordinates, renders it to the screen as a line.
	 *  
	 *	\param x1 The x value of the first point of the line to be rendered.
	 *	\param y1 The y value of the first point of the line to be rendered.
	 *	\param x2 The x value of the second point of the line to be rendered.
	 *	\param y2 The y value of the second point of the line to be rendered.
	 *	\param theColor The color of the line.
	 *  \return n/a
	 */
	static void drawLine(const float x1, const float y1, const float x2, const float y2, color theColor);

	/*! \fn drawTriangle(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, color theColor, BOOL bFilled)
	 *  \brief Takes the coordinates of three points, and renders a triangle out of them.
	 *  
	 *	\param x1 The x value of the first point of the triangle to be rendered.
	 *	\param y1 The y value of the first point of the triangle to be rendered.
	 *	\param x2 The x value of the second point of the triangle to be rendered.
	 *	\param y2 The y value of the second point of the triangle to be rendered.
	 *	\param x3 The x value of the third point of the triangle to be rendered.
	 *	\param y3 The y value of the third point of the triangle to be rendered.
	 *	\param theColor The color of the triangle.
	 *	\param bFilled Indicates whether the triangle will be filled or be rendered as a frame. This is TRUE by default.
	 *  \return n/a
	 */
	static void drawTriangle(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, color theColor, BOOL bFilled = TRUE);

	/*! \fn drawRect(const float x, const float y, const float w, const float h, color theColor, BOOL bFilled = TRUE)
	 *  \brief Renders a rectangle on the screen, given a point and a width and height.
	 *  
	 *	\param x The x location of the rectangle.
	 *	\param y The y location of the rectangle.
	 *	\param w The width of the rectangle.
	 *	\param h The height of the rectangle.
	 *	\param theColor The color of the rectangle.
	 *	\param bFilled Indicates whether the rectangle will be filled or be rendered as a frame. This is TRUE by default.
	 *  \return n/a
	 */
	static void drawRect(const float x, const float y, const float w, const float h, color theColor, BOOL bFilled = TRUE);
	
	/*! \fn drawRect(colorRect colRect, BOOL bFilled = TRUE)
	 *  \brief Renders a rectangle on the screen.
	 *  
	 *	\param colRect The colored rectangle.
	 *	\param bFilled Indicates whether the rectangle will be filled or be rendered as a frame. This is TRUE by default.
	 *  \return n/a
	 */
	static void drawRect(colorRect colRect, BOOL bFilled = TRUE);

	/*! \fn draw3DRect(const float x, const float y, const float z, const float w, const float h, color theColor, BOOL bFilled = TRUE)
	 *  \brief Renders a rectangle on the screen, given a point and a width and height.
	 *  
	 * This is rendered in the 3D view space.
	 *	\param x The x location of the rectangle.
	 *	\param y The y location of the rectangle.
	 *	\param z The z location of the rectangle.
	 *	\param w The width of the rectangle.
	 *	\param h The height of the rectangle.
	 *	\param theColor The color of the rectangle.
	 *	\param bFilled Indicates whether the rectangle will be filled or be rendered as a frame. This is TRUE by default.
	 *  \return n/a
	 */
	static void draw3DRect(const float x, const float y, const float z, const float w, const float h, color theColor, BOOL bFilled = TRUE);
	
	/*! \fn draw3DRect(colorRect colRect, BOOL bFilled = TRUE)
	 *  \brief Renders a rectangle on the screen.
	 *  
	 * This is rendered in the 3D view space.
	 *	\param colRect The colored rectangle.
	 *	\param bFilled Indicates whether the rectangle will be filled or be rendered as a frame. This is TRUE by default.
	 *  \return n/a
	 */
	static void draw3DRect(colorRect colRect, BOOL bFilled);
	
	/*! \fn drawEllipse (GLfloat x, GLfloat y, GLfloat w, GLfloat h, const int segments,  color theColor, BOOL bFilled = TRUE)
	 *  \brief Renders an ellipse on the screen, given a center point and a width and height.
	 *  
	 *	\param x The center x location of the ellipse.
	 *	\param y The center y location of the ellipse.
	 *	\param w The width of the ellipse.
	 *	\param h The height of the ellipse.
	 *	\param segments The number of segments used to construct the ellipse. Higher values results in better a looking ellipse, but at a cost of performance.
	 *	\param theColor The color of the ellipse.
	 *	\param bFilled Indicates whether the ellipse will be filled or be rendered as a frame. This is TRUE by default.
	 *  \return n/a
	 */
	static void drawEllipse (GLfloat x, GLfloat y, GLfloat w, GLfloat h, const int segments,  color theColor, BOOL bFilled = TRUE);

	/*! \fn drawCircle (GLfloat x, GLfloat y, GLfloat radius, const int segments,  color theColor, BOOL bFilled = TRUE)
	 *  \brief Renders a circle on the screen, given a center point and a radius.
	 *  
	 *	\param x The center x location of the circle.
	 *	\param y The center y location of the circle.
	 *	\param radius The radius of the circle.
	 *	\param segments The number of segments used to construct the circle. Higher values results in better a looking circle, but at a cost of performance.
	 *	\param theColor The color of the circle.
	 *	\param bFilled Indicates whether the circle will be filled or be rendered as a frame. This is TRUE by default.
	 *  \return n/a
	 */
	static void drawCircle (GLfloat x, GLfloat y, GLfloat radius, const int segments,  color theColor, BOOL bFilled = TRUE);
	
	/*! \fn drawCircleSlice(GLfloat x, GLfloat y, GLfloat radius, const int segments, GLfloat startDeg, GLfloat endDeg, color theColor)
	 *  \brief Renders a circle slice on the screen.
	 *  
	 * This is used mostly as a helper function.
	 *	\param x The x location of the point of the circle slice.
	 *	\param y The y location of the point of the circle slice.
	 *	\param radius The radius of the circle slice.
	 *	\param segments The number of segments used to construct the circle slice. Higher values results in better a looking circle slice, but at a cost of performance.
	 *	\param theColor The color of the circle slice.
	 *  \return n/a
	 */
	static void drawCircleSlice(GLfloat x, GLfloat y, GLfloat radius, const int segments, GLfloat startDeg, GLfloat endDeg, color theColor);

	/*! \fn drawRoundedRect(const float x, const float y, const float w, const float h, GLfloat cornerRadius, color theColor)
	 *  \brief Renders a rounded rectangle on the screen.
	 *  
	 *	\param x The x location of the rounded rectangle.
	 *	\param y The y location of the rounded rectangle.
	 *	\param w The width of the rounded rectangle.
	 *	\param h The height of the rounded rectangle.
	 *	\param cornerRadius The radius of the corners of the rounded rectangle.
	 *	\param theColor The color of the rounded rectangle.
	 *  \return n/a
	 */
	static void drawRoundedRect(const float x, const float y, const float w, const float h, GLfloat cornerRadius, color theColor);
	
	/*! \fn drawRoundedRect(const float x, const float y, const float w, const float h, color theColor)
	 *  \brief Renders a rounded rectangle on the screen.
	 *  
	 * This uses a default rounded corner radius.
	 *	\param x The x location of the rounded rectangle.
	 *	\param y The y location of the rounded rectangle.
	 *	\param w The width of the rounded rectangle.
	 *	\param h The height of the rounded rectangle.
	 *	\param theColor The color of the rounded rectangle.
	 *  \return n/a
	 */
	static void drawRoundedRect(const float x, const float y, const float w, const float h, color theColor);
	
	/*! \fn drawRoundedRect(colorRect pRect)
	 *  \brief Renders a rounded rectangle on the screen.
	 *  
	 * This uses a default rounded corner radius.
	 *	\param colRect The colored rectangle.
	 *  \return n/a
	 */
	static void drawRoundedRect(colorRect pRect);

	/*! \fn draw3DSpriteCentered(const CSprite* sprite)
	 *  \brief Renders a sprite on the screen in the 3D view space.
	 *  
	 * The sprite is rendered using the sprite's location values and automatically centered using the sprite's size.
	 *	\param sprite The sprite to be rendered.
	 *  \return n/a
	 */	
	static void draw3DSpriteCentered(const CSprite* sprite);
	
	/*! \fn draw3DSpriteCentered(const CSprite* sprite, const float x, const float y, const float z)
	 *  \brief Renders a sprite on the screen in the 3D view space and automatically centers itself using the sprite's size.
	 *  
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
 	 *	\param z The z location where the sprite will be rendered.
	 *  \return n/a
	 */
	static void draw3DSpriteCentered(const CSprite* sprite, const float x, const float y, const float z);	
	
	/*! \fn draw3DSprite(const CSprite* sprite)
	 *  \brief Renders a sprite on the screen in the 3D view space.
	 *  
	 * The sprite is rendered using the sprite's location values.
	 *	\param sprite The sprite to be rendered.
	 *  \return n/a
	 */
	static void draw3DSprite(const CSprite* sprite);
	
	/*! \fn draw3DSprite(const CSprite* sprite, const float x, const float y, const float z)
	 *  \brief Renders a sprite on the screen in the 3D view space.
	 *  
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
 	 *	\param z The z location where the sprite will be rendered.
	 *  \return n/a
	 */
	static void draw3DSprite(const CSprite* sprite, const float x, const float y, const float z);
	
	/*! \fn draw3DSprite(const CSprite* sprite, const float x, const float y, const float z, const float offsetX, const float offsetY, const float clipW, const float clipH)
	 *  \brief Renders a sprite on the screen.
	 *  
	 * This function allows for setting an internal clipping location and area of the sprite.
	 * This is useful for rendering sprites from a sprite table.
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
	 *	\param z The z location where the sprite will be rendered.
	 *	\param offsetX The x offset in the sprite where the blit will begin.
	 *	\param offsetY The y offset in the sprite where the blit will begin.
	 *	\param clipW The clip width of the sprite blit.
	 *	\param clipH The clip height of the sprite blit.
	 *  \return n/a
	 */
	static void draw3DSprite(const CSprite* sprite, const float x, const float y, const float z, const float offsetX, const float offsetY, const float clipW, const float clipH);
	
	/*! \fn draw3DSpriteLookAt(const CSprite* sprite, const float x, const float y, const float z, const float* camMat)
	 *  \brief Renders a sprite on the screen, always front-facing the camera.
	 *  
	 *  The sprite will be rendered from the upper-left hand corner.
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
	 *	\param z The z location where the sprite will be rendered.
	 *	\param camMat The camera view matrix, used to construct the vertices array for the sprite, in order to front-face it towards the camera.
	 *  \return n/a
	 */
	static void draw3DSpriteLookAt(const CSprite* sprite, const float x, const float y, const float z, const float* camMat);
	
	/*! \fn draw3DSpriteCenteredLookAt(const CSprite* sprite, const float* camMat)
	 *  \brief Renders a sprite on the screen, centered, and always front-facing the camera.
	 *  
	 *	\param sprite The sprite to be rendered.
	 *	\param camMat The camera view matrix, used to construct the vertices array for the sprite, in order to front-face it towards the camera.
	 *  \return n/a
	 */
	static void draw3DSpriteCenteredLookAt(const CSprite* sprite, const float* camMat);
	
	/*! \fn draw3DSpriteCenteredLookAt(const CSprite* sprite, const float x, const float y, const float z, const float* camMat)
	 *  \brief Renders a sprite on the screen, centered, and always front-facing the camera.
	 *  
	 *  The sprite will be rendered from the center, with the given coordinates.
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
	 *	\param z The z location where the sprite will be rendered.
	 *	\param camMat The camera view matrix, used to construct the vertices array for the sprite, in order to front-face it towards the camera.
	 *  \return n/a
	 */
	static void draw3DSpriteCenteredLookAt(const CSprite* sprite, const float x, const float y, const float z, const float* camMat);
	
	
	
	static void drawSpriteCentered(const CSprite &sprite);
	static void drawSpriteCentered(const CSprite &sprite, const float x, const float y);
	static void drawSprite(const CSprite &sprite);
	static void drawSprite(const CSprite &sprite, const float x, const float y);
	static void drawSprite(const CSprite &sprite, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH);
	static void draw3DSprite(const CSprite &sprite);
	static void draw3DSprite(const CSprite &sprite, const float x, const float y, const float z);
	static void draw3DSprite(const CSprite &sprite, const float x, const float y, const float z, const float offsetX, const float offsetY, const float clipW, const float clipH);
	static void draw3DSpriteCenteredLookAt(const CSprite& sprite, const float* camMat);
	static void draw3DSpriteCenteredLookAt(const CSprite& sprite, const float x, const float y, const float z, const float* camMat);
	
	
	/*! \fn drawSprite(const CSprite* sprite, const float x, const float y)
	 *  \brief Renders a sprite on the screen.
	 *  
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
	 *  \return n/a
	 */
	static void drawSprite(const CSprite* sprite, const float x, const float y);
	
	/*! \fn drawSprite(const CSprite* sprite)
	 *  \brief Renders a sprite on the screen.
	 *  
	 * The sprite is rendered using the sprite's location values.
	 *	\param sprite The sprite to be rendered.
	 *  \return n/a
	 */
	static void drawSprite(const CSprite* sprite);
	
	/*! \fn drawSprite(const CSprite* sprite, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH)
	 *  \brief Renders a sprite on the screen.
	 *  
	 * This function allows for setting an internal clipping location and area of the sprite.
	 * This is useful for rendering sprites from a sprite table.
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
	 *	\param offsetX The x offset in the sprite where the blit will begin.
	 *	\param offsetY The y offset in the sprite where the blit will begin.
	 *	\param clipW The clip width of the sprite blit.
	 *	\param clipH The clip height of the sprite blit.
	 *  \return n/a
	 */
	static void drawSprite(const CSprite* sprite, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH);

	/*! \fn drawSpriteCentered(const CSprite* sprite)
	 *  \brief Renders a sprite on the screen.
	 *  
	 * The sprite is rendered using the sprite's location values and automatically centered using the sprite's size.
	 *	\param sprite The sprite to be rendered.
	 *  \return n/a
	 */	
	static void drawSpriteCentered(const CSprite* sprite);
	
	/*! \fn drawSpriteCentered(const CSprite* sprite, const float x, const float y)
	 *  \brief Renders a sprite on the screen and automatically centers itself using the sprite's size.
	 *  
	 *	\param sprite The sprite to be rendered.
	 *	\param x The x location where the sprite will be rendered.
	 *	\param y The y location where the sprite will be rendered.
	 *  \return n/a
	 */
	static void drawSpriteCentered(const CSprite* sprite, const float x, const float y);
	
	/*! \fn bindImage(const CImage* image)
	 *  \brief Binds an already loaded image to a texture name.
	 *  
	 *	\param image The image that will be binded to a texture name.
	 *  \return n/a
	 */
	static void bindImage(const CImage* image);
	
	/*! \fn drawImage(const CImage* image, const float x, const float y, GLuint texName)
	 *  \brief Renders an image on the screen.
	 *  
	 *	\param image The image to be rendered.
	 *	\param x The x location where the image will be rendered.
	 *	\param y The y location where the image will be rendered.
 	 *	\param texName OGL texture name to bind the correct texure for the quad to be rendered.
	 *  \return n/a
	 */
	static void drawImage(const CImage* image, const float x, const float y, GLuint texName);

	/*! \fn drawImage(const CImage* image, const float x, const float y, GLuint texName, const float alpha)
	 *  \brief Renders an image on the screen.
	 *  
	 *	\param image The image to be rendered.
	 *	\param x The x location where the image will be rendered.
	 *	\param y The y location where the image will be rendered.
	 *	\param texName OGL texture name to bind the correct texure for the quad to be rendered.
	 *	\param alpha The alpha value to apply to this image when rendered. 1.0 is fully opaque, while 0.0 is fully transparent.
	 *  \return n/a
	 */
	static void drawImage(const CImage* image, const float x, const float y, GLuint texName, const float alpha);

	/*! \fn drawImage(const CImage* image, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH)
	 *  \brief Renders an image on the screen.
	 *  
	 * This function allows for setting an internal clipping location and area of the image.
	 * This is useful for rendering images from a sprite table.
	 *	\param image The image to be rendered.
	 *	\param x The x location where the image will be rendered.
	 *	\param y The y location where the image will be rendered.
	 *	\param offsetX The x offset in the image where the blit will begin.
	 *	\param offsetY The y offset in the image where the blit will begin.
	 *	\param clipW The clip width of the image blit.
	 *	\param clipH The clip height of the image blit.
	 *  \return n/a
	 */
	static void drawImage(const CImage* image, const float x, const float y, const float offsetX, const float offsetY, const float clipW, const float clipH);

	/*! \fn buildTriangleVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
	 *  \brief Constructs a quad vertex array using two triangles.
	 *  
	 * Vertex arrays are used to render primitive shapes by utilizing the openGL glDrawArrays function.
	 *	\param vertices The pointer to the vertex array which will be filled with vertex data constructed by two triangles.
	 *	\param x The top left x location of the quad.
	 *	\param y The top left y location of the quad.
	 *	\param w The width of the quad.
	 *	\param h The height of the quad.
	 *  \return n/a
	 */
	static void buildTriangleVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h);

	/*! \fn buildTriangleVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h)
	 *  \brief Constructs a quad vertex array using two triangles. Used for 3D mode.
	 *  
	 * Vertex arrays are used to render primitive shapes by utilizing the openGL glDrawArrays function.
	 *	\param vertices The pointer to the vertex array which will be filled with vertex data constructed by two triangles.
	 *	\param x The top left x location of the quad.
	 *	\param y The top left y location of the quad.
 	 *	\param z The top left z location of the quad.
	 *	\param w The width of the quad.
	 *	\param h The height of the quad.
	 *  \return n/a
	 */
	static void buildTriangleVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h);
	
	/*! \fn buildQuadVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
	 *  \brief Constructs a quad vertex array.
	 *  
	 * Vertex arrays are used to render primitive shapes by utilizing the openGL glDrawArrays function.
	 *	\param vertices The pointer to the vertex array which will be filled with vertex data.
	 *	\param x The top left x location of the quad.
	 *	\param y The top left y location of the quad.
	 *	\param w The width of the quad.
	 *	\param h The height of the quad.
	 *  \return n/a
	 */
	static void buildQuadVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h);

	/*! \fn buildQuadVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h)
	 *  \brief Constructs a quad vertex array. Used for 3D mode.
	 *  
	 * Vertex arrays are used to render primitive shapes by utilizing the openGL glDrawArrays function.
	 *	\param vertices The pointer to the vertex array which will be filled with vertex data.
	 *	\param x The top left x location of the quad.
	 *	\param y The top left y location of the quad.
 	 *	\param z The top left z location of the quad.
	 *	\param w The width of the quad.
	 *	\param h The height of the quad.
	 *  \return n/a
	 */
	static void buildQuadVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h);
	
	/*! \fn buildQuadLineLoopVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
	 *  \brief Constructs a quad vertex array in such a manner that rendering the frame of the shape will not look distorted.
	 *  
	 * Vertex arrays are used to render primitive shapes by utilizing the openGL glDrawArrays function.
	 *	\param vertices The pointer to the vertex array which will be filled with vertex data.
	 *	\param x The top left x location of the quad.
	 *	\param y The top left y location of the quad.
	 *	\param w The width of the quad.
	 *	\param h The height of the quad.
	 *  \return n/a
	 */
	static void buildQuadLineLoopVertices(GLfloat* vertices, GLfloat x, GLfloat y, GLfloat w, GLfloat h);

	/*! \fn buildQuadTexCoords(GLfloat* texCoords, const float x, const float y, const float w, const float h, const CImage* image)
	 *  \brief Constructs a quad texture array.
	 *  
	 * Texture arrays are used to map textures onto a vertex array.
	 *	\param texCoords The pointer to the texture coordinate array to be filled with texture coordinate data.
	 *	\param x The top left x location of the quad.
	 *	\param y The top left y location of the quad.
	 *	\param w The width of the quad.
	 *	\param h The height of the quad.
	 *	\param image The texture to be mapped.
	 *  \return n/a
	 */
	static void buildQuadTexCoords(GLfloat* texCoords, const float x, const float y, const float w, const float h, const CImage* image);
	
	/*! \fn buildTriangleTexCoords(GLfloat* texCoords, const float x, const float y, const float w, const float h, const CImage* image)
	 *  \brief Constructs a quad texture array using 2 gl triangles.
	 *  
	 * Texture arrays are used to map textures onto a vertex array.
	 *	\param texCoords The pointer to the texture coordinate array to be filled with texture coordinate data.
	 *	\param x The top left x location of the quad.
	 *	\param y The top left y location of the quad.
	 *	\param w The width of the quad.
	 *	\param h The height of the quad.
	 *	\param image The texture to be mapped.
	 *  \return n/a
	 */
	static void buildTriangleTexCoords(GLfloat* texCoords, const float x, const float y, const float w, const float h, const CImage* image);
};


extern const GLfloat tex_coords[];	/*!< These are used to map a texture onto a surface. Depending on which method we use (libpng or coregraphics), this will be different. */

#endif