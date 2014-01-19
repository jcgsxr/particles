/*
 *  Image.h
 *  framework
 *
 *  Created by Johnny Chan on 10/9/09.
 *  Copyright 2009 Llamaface. All rights reserved.
 *
 */
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <OpenGLES/ES1/gl.h>


/*! \class CImage
 * \brief The Image class.
 *
 * The Image class is responsible for loading and holding image data loaded from the disk. It does not contain any rendering functionality; That is the responsibility of the CGraphics class.
 */
class CImage
{
private:
#if defined (ENABLE_PNGLOAD)
	char* _image_data;
#else
	GLubyte* _image_data;
#endif
	unsigned long _width;	/*!< The pixel width of the image. */
	unsigned long _height;	/*!< The pixel height of the image. */
	int _gl_format;			/*!< Retrieved from pngLoad, will either be GL_RGB or GL_RGBA. */
	
public:
	
	/*! \fn CImage()
	 *  \brief The CImage class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CImage();
	
	/*! \fn ~CImage()
	 *  \brief The CImage class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CImage();
	
	/*! \fn getWidth()
	 *  \brief Returns the pixel width of the image.
	 *  
	 *	\param n/a
	 *  \return The pixel width of the image.
	 */
	inline unsigned long getWidth(void) const {return _width;}
	
	/*! \fn getHeight()
	 *  \brief Returns the pixel height of the image.
	 *  
	 *	\param n/a
	 *  \return The pixel height of the image.
	 */
	inline unsigned long getHeight(void) const {return _height;}
	
	/*! \fn getGLFormat()
	 *  \brief Returns the data format of the image.
	 *  
	 * The data format will either be GL_RGB or GL_RGBA.
	 *	\param n/a
	 *  \return The data format of the image.
	 */
	inline int getGLFormat(void) const {return _gl_format;}
	
#if defined (ENABLE_PNGLOAD)
	/*! \fn getImageData()
	 *  \brief Returns the image data of the loaded image.
	 *  
	 *	\param n/a
	 *  \return The image data of the loaded image.
	 */
	inline char* getImageData(void) const {return _image_data;}
#else
	/*! \fn getImageData()
	 *  \brief Returns the image data of the loaded image.
	 *  
	 *	\param n/a
	 *  \return The image data of the loaded image.
	 */
	inline GLubyte* getImageData(void) const {return _image_data;}
#endif
	
	/*! \fn load(const char* fileName)
	 *  \brief Takes an image from disk and loads it into memory.
	 *  
	 *	\param fileName The file name of the image to be loaded.
	 *  \return n/a
	 */
	void load(const char* fileName);
	
	/*! \fn unload()
	 *  \brief Deallocates the loaded image data.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void unload(void);
};


#endif