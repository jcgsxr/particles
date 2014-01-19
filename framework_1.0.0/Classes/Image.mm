#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "Image.h"
#include "Utils.h"
//#include "SystemDefines.h"

CImage::CImage()
{
	
}

CImage::~CImage()
{
	unload();
}

void CImage::load(const char* fileName)
{
	char* file_name = (char*)fileName;
	// Parse out the filename and extension type from the file_name passed in
	NSString *nsFullFileName = [NSString stringWithUTF8String:file_name];
	NSString *nsFileName = [nsFullFileName stringByDeletingPathExtension];
	NSString *fileType = [nsFullFileName pathExtension];
	
	NSString* filePath;
	// Get the absolute path to the file
	filePath = [[NSBundle mainBundle] pathForResource:nsFileName ofType:fileType];
	
#if defined (ENABLE_PNGLOAD)
	
	// Get the path as a c string so we can open it with fopen()
	file_name = (char*)[filePath UTF8String];
	
	int res = 0;
	
	// This is where the texture data gets loaded.  
	res = pngLoad(file_name, &_width, &_height, &_gl_format, &_image_data);
	
#if defined (DEBUG_IMAGE_ERROR)
	if (res != 1)
	{
		DPRINT_IMAGE("CImage::load pngLoad failed");
	}
#endif
	//res = pngLoad(file_name, &_width, &_height, &_image_data);
	
#else
	CGImageRef spriteImage;
	CGContextRef spriteContext;
	GLubyte *spriteData;
	size_t width, height;
	
	// Creates a Core Graphics image from an image file
	spriteImage = [UIImage imageNamed:nsFullFileName].CGImage;
	// Get the width and height of the image
	width = CGImageGetWidth(spriteImage);
	height = CGImageGetHeight(spriteImage);
	_width = (unsigned long) width;
	_height = (unsigned long) height;
	
	_gl_format = GL_RGBA;
	
	if(spriteImage) 
	{
		// Allocated memory needed for the bitmap context
		spriteData = (GLubyte *) calloc(width * height * 4, sizeof(GLubyte));
		// Uses the bitmap creation function provided by the Core Graphics framework. 
		spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width * 4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
		// After you create the context, you can draw the sprite image to the context.
		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
		// You don't need the context at this point, so you need to release it to avoid memory leaks.
		CGContextRelease(spriteContext);
		
		_image_data = spriteData;
		
		// Release the image data
		//free(spriteData);
	}
#endif
}


void CImage::unload()
{
	if (_image_data)
	{
		delete _image_data;
		_image_data = NULL;
	}
}