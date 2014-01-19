#ifndef __TYPES_H__
#define __TYPES_H__

#import <OpenGLES/ES1/gl.h>

/*! \struct point
 *	\brief The point type.
 *
 * A point is composed of x, y, and z integer values.
 */
typedef struct point
{
	int x;
	int y;
	int z;
	
	point operator + (int val)	{	point temp;	temp.x = x + val;	temp.y = y + val;	temp.z = z + val;	return temp;	}
	point operator - (int val)	{	point temp;	temp.x = x - val;	temp.y = y - val;	temp.z = z - val;	return temp;	}
	point operator * (int val)	{	point temp;	temp.x = x * val;	temp.y = y * val;	temp.z = z * val;	return temp;	}
	point operator / (int val)	{	point temp;	temp.x = x / val;	temp.y = y / val;	temp.z = z / val;	return temp;	}
	void operator = (GLfloat val)	{	x = val;	y = val;	z = val;	}
	point operator + (GLfloat val)	{	point temp;	temp.x = x + val;	temp.y = y + val;	temp.z = z + val;	return temp;	}
	point operator - (GLfloat val)	{	point temp;	temp.x = x - val;	temp.y = y - val;	temp.z = z - val;	return temp;	}
	point operator * (GLfloat val)	{	point temp;	temp.x = x * val;	temp.y = y * val;	temp.z = z * val;	return temp;	}
	point operator / (GLfloat val)	{	point temp;	temp.x = x / val;	temp.y = y / val;	temp.z = z / val;	return temp;	}	
	void operator += (GLfloat val)	{	x += val;	y += val;	z += val;	}
	void operator -= (GLfloat val)	{	x -= val;	y -= val;	z -= val;	}
	void operator *= (GLfloat val)	{	x *= val;	y *= val;	z *= val;	}
	void operator /= (GLfloat val)	{	x /= val;	y /= val;	z /= val;	}
	void operator = (point pt)	{	x = pt.x;	y = pt.y;	z = pt.z;	}
	point operator + (point pt)	{	point temp;	temp.x = x + pt.x;	temp.y = y + pt.y;	temp.z = z + pt.z;	return temp;	}
	point operator - (point pt)	{	point temp;	temp.x = x - pt.x;	temp.y = y - pt.y;	temp.z = z - pt.z;	return temp;	}
	point operator * (point pt)	{	point temp;	temp.x = x * pt.x;	temp.y = y * pt.y;	temp.z = z * pt.z;	return temp;	}
	point operator / (point pt)	{	point temp;	temp.x = x / pt.x;	temp.y = y / pt.y;	temp.z = z / pt.z;	return temp;	}
	void operator += (point pt)	{	x += pt.x;	y += pt.y;	z += pt.z;	}
	void operator -= (point pt)	{	x -= pt.x;	y -= pt.y;	z -= pt.z;	}
	void operator *= (point pt)	{	x *= pt.x;	y *= pt.y;	z *= pt.z;	}
	void operator /= (point pt)	{	x /= pt.x;	y /= pt.y;	z /= pt.z;	}
} point;

/*! \struct color
 *	\brief The color type.
 *
 * A color is composed of a red, green, blue, and alpha value. These values range from 0.0 to 1.0
 */
typedef struct color
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
	
	void operator = (color col)	{	r = col.r;	g = col.g;	b = col.b;	a = col.a;	}	
	void operator = (GLfloat val)	{	r = val;	g = val;	b = val;	a = val;	}
	color operator + (GLfloat val)	{	color temp;	temp.r = r + val;	temp.g = g + val;	temp.b = b + val;	temp.a = a + val;	return temp;	}
	color operator - (GLfloat val)	{	color temp;	temp.r = r - val;	temp.g = g - val;	temp.b = b - val;	temp.a = a - val;	return temp;	}
	color operator * (GLfloat val)	{	color temp;	temp.r = r * val;	temp.g = g * val;	temp.b = b * val;	temp.a = a * val;	return temp;	}
	color operator / (GLfloat val)	{	color temp;	temp.r = r / val;	temp.g = g / val;	temp.b = b / val;	temp.a = a / val;	return temp;	}
	color operator + (int val)	{	color temp;	temp.r = r + val;	temp.g = g + val;	temp.b = b + val;	temp.a = a + val;	return temp;	}
	color operator - (int val)	{	color temp;	temp.r = r - val;	temp.g = g - val;	temp.b = b - val;	temp.a = a - val;	return temp;	}
	color operator * (int val)	{	color temp;	temp.r = r * val;	temp.g = g * val;	temp.b = b * val;	temp.a = a * val;	return temp;	}
	color operator / (int val)	{	color temp;	temp.r = r / val;	temp.g = g / val;	temp.b = b / val;	temp.a = a / val;	return temp;	}
	color operator + (color col)	{	color temp;	temp.r = r + col.r;	temp.g = g + col.g;	temp.b = b + col.b;	temp.a = a + col.a;	return temp;	}
	color operator - (color col)	{	color temp;	temp.r = r - col.r;	temp.g = g - col.g;	temp.b = b - col.b;	temp.a = a - col.a;	return temp;	}
	color operator * (color col)	{	color temp;	temp.r = r * col.r;	temp.g = g * col.g;	temp.b = b * col.b;	temp.a = a * col.a;	return temp;	}
	color operator / (color col)	{	color temp;	temp.r = r / col.r;	temp.g = g / col.g;	temp.b = b / col.b;	temp.a = a / col.a;	return temp;	}
	void operator += (color col)	{	r += col.r;	g += col.g;	b += col.b;	a += col.a;	}
	void operator -= (color col)	{	r -= col.r;	g -= col.g;	b -= col.b;	b -= col.a;	}
	void operator *= (color col)	{	r *= col.r;	g *= col.g;	b *= col.b;	a *= col.a;	}
	void operator /= (color col)	{	r /= col.r;	g /= col.g;	b /= col.b;	a /= col.a;	}
} color;

/*! \struct rect
 *	\brief The rectangle type.
 *
 * A rect is composed of a x and y coordinate and a width and height value.
 */
typedef struct rect
{
	long x;
	long y;
	long w;
	long h;
} rect;

/*! \struct colorRect
 *	\brief The color rectangle type.
 *
 * A color rectangle is composed of a red, green, blue, and alpha value, and a x and y coordinate and a width and height value.
 */
typedef struct colorRect
{
	float x;
	float y;
	float z;
	float w;
	float h;

	color col;
} colorRect;


// Change any GL_QUADS drawing into GL_TRIANGLE_STRIP.
#define GL_QUADS		GL_TRIANGLE_STRIP

typedef unsigned int 	uint32; 
typedef signed int 		int32; 
typedef unsigned short 	uint16; 
typedef signed short	int16; 
typedef unsigned char 	uint8; 
typedef signed char		int8; 
typedef unsigned short	unichar;
typedef uint32			RGBVal;
typedef unsigned char	u8;
typedef signed char		s8;
typedef unsigned short	u16; 
typedef signed short	s16; 
typedef unsigned int	u32; 
typedef signed int		s32; 
typedef signed char		BOOL; 
typedef signed short	byte;

#ifdef NULL
#undef NULL
#endif

#define NULL 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE (1)

#ifdef FALSE
#undef FALSE
#endif
#define FALSE (0)

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

static const int __INF = -12345;
static const int ONE_SECOND = 1000;

// for detecting swiping
#define HORIZ_SWIPE_DRAG_MIN 5 
#define VERT_SWIPE_DRAG_MIN 5

/*! \enum eSwipeDirection
 *	\brief The possible swipe directions.
 *
 * The direction values are set up so that bit operations may be performed on them.
 */
typedef enum eSwipeDirection
{
	SWIPE_NONE,				/*!< No swipe direction. */
	SWIPE_UP = 0x000001,	/*!< Up swipe direction. */
	SWIPE_DOWN = 0x000002,	/*!< Down swipe direction. */
	SWIPE_LEFT = 0x000004,	/*!< Left swipe direction. */
	SWIPE_RIGHT = 0x000008,	/*!< Right swipe direction. */
} eSwipeDirection;

/*! \enum eTouchPhase
 *	\brief The possible touch states.
 */
typedef enum eTouchPhase
{
	TouchPhaseBegan,		/*!< The initial touch pressed state. */
	TouchPhaseMoved,		/*!< The touch holding while moving state. */
	TouchPhaseStationary,	/*!< The touch holding without moving state. */
	TouchPhaseEnded,		/*!< The touch released state. */
	TouchPhaseCanceled,		/*!< General touch state that indicates that touch values should be cleared. */
	TouchPhaseMulti,		/*!< Indicates that there has been multiple touches on the screen. */
	TouchPhaseMAX,
} eTouchPhase;


//#define RGBA(r, g, b, a)	[UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a]

#define RGB(r, g, b)		[UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1]

#define RGBA(x)				[UIColor colorWithRed:((float)((x & 0xFF000000) >> 24) / 255) green:((float)((x & 0x00FF0000) >> 16) / 255) blue:((float)((x & 0x0000FF00) >> 8) / 255) alpha:((float)(x & 0x000000FF) / 255)]


// This is used when reading byte streams that contains floats.
union FloatHex
{
	float floatVal;
	int hexVal;
};

#endif
	
