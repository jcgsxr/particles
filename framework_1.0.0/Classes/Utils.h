#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"

#define TEXT_VIEW_FONT_SIZE		16.0f

// Used for loading png image files using the libpng library.
extern int pngLoad(char *file, unsigned long *pwidth, unsigned long *pheight, int* pFormat, char **image_data_ptr);

// Collision function used to determine whether a point lies withing a rectangle.
extern BOOL isPointInRect(float px, float py, float rx, float ry, float rw, float rh);
extern BOOL isPointInRect(float px, float py, colorRect rectangle);
extern BOOL isPointInRect(float px, float py, rect rectangle);

// Convenience function for initializing a rect.
extern void initRect(rect &theRect, const long x, const long y, const long w, const long h); 

// Returns the distance between two points.
extern float distBetweenPts(point pt1, point pt2); 
extern float distBetweenPts(float x1, float y1, float x2, float y2);

// Normalizes a vector.
extern void mathsNormalize(float *v);

// Converts 3d view coordinates to screen coordinates.
extern void coords3DToScreen(float x, float y, float z, float* resX, float* resY, float* resZ);

// Converts screen coordinates to 3d view coordinates.
extern void coordsScreenTo3D(float x, float y, float z, float* resX, float* resY, float* resZ);


// Time conversion functions.
extern int getSeconds(int millisecs);
extern int getMinutes(int millisecs);
extern int getHours(int millisecs);


/*! \fn gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zmin, GLfloat zmax)
 *  \brief Sets up a 3D perspective view.
 *  
 *	\param fovy The field of view angle. Usually 45 degrees yields a regular field of view.
 *	\param aspect The aspect ratio of the view. Usually screen width divided by screen height.
 *	\param zmin The locaiton of the near clipping plane.
 *	\param zmax The location of the far clipping plane.
 *  \return n/a
 */
extern void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zmin, GLfloat zmax);

/*! \fn gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy, GLfloat upz)
 *  \brief Manual gluLookAt implementation.
 *  
 *	\param eyex The camera x position.
 *	\param eyey The camera y position.
 *	\param eyez The camera z position.
 *	\param centerx The point of interest x position.
 *	\param centery The point of interest y position.
 *	\param centerz The point of interest z position.
 *	\param upx The up vector x value.
 *	\param upy The up vector y value.
 *	\param upz The up vector z value.
 *  \return n/a
 */
extern void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
					  GLfloat centerx, GLfloat centery, GLfloat centerz,
					  GLfloat upx, GLfloat upy, GLfloat upz);

/*! \fn set3Dview(void)
 *  \brief Prepares the view space to handle rendering a 3D scene.
 *  
 *	\param n/a
 *  \return n/a
 */
extern void set3Dview(void);

/*! \fn set2Dview(void)
 *  \brief Prepares the view space to handle rendering a 2D scene.
 *  
 *	\param n/a
 *  \return n/a
 */
extern void set2Dview(void);

#endif