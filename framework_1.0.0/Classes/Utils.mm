//#include "png.h"
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h" 
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "SystemDefines.h"




void mathsNormalize(float *v) 
{
	float d = (sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2])));
	v[0] = v[0] / d;
	v[1] = v[1] / d;
	v[2] = v[2] / d;
}

void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zmin, GLfloat zmax)
{
	GLfloat xmin, xmax, ymin, ymax;
	ymax = zmin * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	glFrustumf(xmin, xmax, ymin, ymax, zmin, zmax);
}

void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
			   GLfloat centerx, GLfloat centery, GLfloat centerz,
			   GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    glMultMatrixf(m);
    
    /* Translate Eye to Origin */
    glTranslatef(-eyex, -eyey, -eyez);
    
}


void set2Dview(void)
{
	// Set up view properties.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Notice that the top and bottom values are reversed.  This is to match the coordinate system of the iphone.
	glOrthof(
			 0.0f,			// Left 
			 (float)SCRN_W,	// Right
			 (float)SCRN_H,	// Bottom
			 0.0f,			// Top
			 -1.0f,			// Near val
			 1.0f);			// Far val	
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

void set3Dview(void)
{
	// Set up view properties.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Now set up the perspective view.
	gluPerspective(
				   PERSPECTIVE_FOVY,		// fovy
				   PERSPECTIVE_ASPECT,		// Apsect.
				   PERSPECTIVE_NEAR_CLIP,	// Near clip.
				   PERSPECTIVE_FAR_CLIP);	// Far clip.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glMatrixMode(GL_MODELVIEW);
}



void coordsScreenTo3D(float x, float y, float z, float* resX, float* resY, float* resZ)
{
	// Translate to the 3d coordinate system.
	*resX = ((float)(x - HALF_SCRN_W)) / HALF_SCRN_W;
	*resY = ((float)(y - HALF_SCRN_H)) / HALF_SCRN_H;
	*resZ = (float)z / PERSPECTIVE_FAR_CLIP;
	
	// Account for the orthographic scale.
	*resX *= ORTHO_RIGHT_3D;
	*resY *= ORTHO_BOTTOM_3D;
}


void coords3DToScreen(float x, float y, float z, float* resX, float* resY, float* resZ)
{
	// Translate to the screen coordinate system.
	*resX = (x * HALF_SCRN_W) + HALF_SCRN_W;
	*resY = (x * HALF_SCRN_W) + HALF_SCRN_W;
	*resZ = z * PERSPECTIVE_FAR_CLIP;
	
	*resX /= ORTHO_RIGHT_3D;
	*resY /= ORTHO_BOTTOM_3D;
}


float distBetweenPts(point pt1, point pt2)
{
	return sqrt(((pt2.x - pt1.x) * (pt2.x - pt1.x)) + ((pt2.y - pt1.y) * (pt2.y - pt1.y)));
}


float distBetweenPts(float x1, float y1, float x2, float y2)
{
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}


#if defined (ENABLE_PNGLOAD)
int pngLoad(char *file, unsigned long *pwidth, unsigned long *pheight, int* pFormat, char **image_data_ptr)
{
	
	FILE         *infile;         /* PNG file pointer */
	png_structp   png_ptr;        /* internally used by libpng */
	png_infop     info_ptr;       /* user requested transforms */
	
	char         *image_data;      /* raw png image data */
	char         sig[8];           /* PNG signature array */
	/*char         **row_pointers;   */
	
	int           bit_depth;
	int           color_type;
	
	unsigned long width;            /* PNG image width in pixels */
	unsigned long height;           /* PNG image height in pixels */
	unsigned int rowbytes;         /* raw bytes at row n in image */
	
	image_data = NULL;
	int i;
	png_bytepp row_pointers = NULL;
	
	/* Open the file. */
	infile = fopen(file, "rb");
	if (!infile) {
		return 0;
	}
	
	
	/*
	 * 		13.3 readpng_init()
	 */
	
	/* Check for the 8-byte signature */
	fread(sig, 1, 8, infile);
	
	if (!png_check_sig((unsigned char *) sig, 8)) {
		fclose(infile);
		return 0;
	}
	
	/* 
	 * Set up the PNG structs 
	 */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(infile);
		return 4;    /* out of memory */
	}
	
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(infile);
		return 4;    /* out of memory */
	}
	
	
	/*
	 * block to handle libpng errors, 
	 * then check whether the PNG file had a bKGD chunk
	 */
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(infile);
		return 0;
	}
	
	/* 
	 * takes our file stream pointer (infile) and 
	 * stores it in the png_ptr struct for later use.
	 */
	/* png_ptr->io_ptr = (png_voidp)infile;*/
	png_init_io(png_ptr, infile);
	
	/*
	 * lets libpng know that we already checked the 8 
	 * signature bytes, so it should not expect to find 
	 * them at the current file pointer location
	 */
	png_set_sig_bytes(png_ptr, 8);
	
	/* Read the image info.*/
	
	/*
	 * reads and processes not only the PNG file's IHDR chunk 
	 * but also any other chunks up to the first IDAT 
	 * (i.e., everything before the image data).
	 */
	
	/* read all the info up to the image data  */
	png_read_info(png_ptr, info_ptr);
	
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
	
	*pwidth = width;
	*pheight = height;
	
	// Set up some transforms.
    if (bit_depth == 16)
    {
        png_set_strip_16(png_ptr);
    }
	if (info_ptr->bit_depth < 8)
	{
		png_set_packing(png_ptr);
	}
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png_ptr);
	}
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
	}
	
	/* Update the png info struct.*/
	png_read_update_info(png_ptr, info_ptr);
	
	int channels = png_get_channels(png_ptr, info_ptr);
	
	if (channels == 4)
	{
		*pFormat = GL_RGBA;
	}
	else
	{
		// Just default to regular rgb.
		*pFormat = GL_RGB;
	}
	
	/* Rowsize in bytes. */
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	
	
	/* Allocate the image_data buffer. */
	if ((image_data = (char *) malloc(rowbytes * height))==NULL) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 4;
    }
	
	if ((row_pointers = (png_bytepp)malloc(height*sizeof(png_bytep))) == NULL) 
	{
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image_data);
        image_data = NULL;
        return 4;
    }
	
	
    /* set the individual row_pointers to point at the correct offsets */
	
    for (i = 0;  i < height;  ++i)
        row_pointers[height - 1 - i] = (png_byte*)(image_data + i*rowbytes);
	
	
    /* now we can go ahead and just read the whole image */
    png_read_image(png_ptr, row_pointers);
	
    /* and we're done!  (png_read_end() can be omitted if no processing of
     * post-IDAT text/time/etc. is desired) */
	
	/* Clean up. */
	free(row_pointers);
	
	/* Clean up. */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(infile);
	
	*image_data_ptr = image_data;
	
	return 1;
}
#endif

BOOL isPointInRect(float px, float py, float rx, float ry, float rw, float rh)
{
	if (px < rx)
	{
		return FALSE;
	}
	if (py < ry)
	{
		return FALSE;
	}
	if (px > rx + rw)
	{
		return FALSE;
	}
	if (py > ry + rh)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL isPointInRect(float px, float py, colorRect rectangle)
{
	return isPointInRect(px, py, rectangle.x, rectangle.y, rectangle.w, rectangle.h);
}

BOOL isPointInRect(float px, float py, rect rectangle)
{
	return isPointInRect(px, py, rectangle.x, rectangle.y, rectangle.w, rectangle.h);
}


void initRect(rect &theRect, const long x, const long y, const long w, const long h)
{
	theRect.x = x;
	theRect.y = y;
	theRect.w = w;
	theRect.h = h;
}


int getSeconds(int millisecs)
{
	return millisecs / 1000;
}

int getMinutes(int millisecs) 
{
	return getSeconds(millisecs) / 60;
}

int getHours(int millisecs) 
{
	return getMinutes(millisecs) / 60;
}