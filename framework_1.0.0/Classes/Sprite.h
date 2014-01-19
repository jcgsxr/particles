#ifndef __SPRITE_H__
#define __SPRITE_H__

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "types.h"
#include "Vector3.h"

static const int SPRITE_ANIM_FRAMES_MAX = 64;

/*! \enum eAnimType
 *	\brief All the possible animation types.
 */
typedef enum eAnimType
{
	ANIM_TYPE_NONE = 0,	/*!< When there is no animation associated with the sprite. */
	ANIM_TYPE_ONCE,		/*!< The animation runs once. */
	ANIM_TYPE_ONCE_FADE,/*!< The animation runs once with a fade between frames. */
	ANIM_TYPE_LOOP,		/*!< Specifies that the animation will loop back to the first frame after completeing it's last frame. */
	ANIM_TYPE_LOOP_FADE,/*!< Specifies that the animation will loop back to the first frame after completeing it's last frame with a fade between frames. */
	ANIM_TYPE_ENDTOEND,	/*!< Specifies that the animation will loop the frames in a back and forth manner so that when the animation reaches the last frame, it will reverse and count backwards until it reaches the first frame again. */
	ANIM_TYPE_ENDTOEND_FADE,	/*!< Specifies that the animation will loop the frames in a back and forth manner so that when the animation reaches the last frame, it will reverse and count backwards until it reaches the first frame again. This occurs with a fade between frames.*/
	ANIM_TYPE_MAX,		/*!< The total amount of animation types. */
} eAnimType;

/*! \enum eAnimDirection
*	\brief Animation direction refers to the direction that frames will progress within an animation.
*/
typedef enum eAnimDirection
{
	ANIM_DIR_POS = 0,	/*!< Progress through animation frames by incrementing. */
	ANIM_DIR_NEG,		/*!< Progress through animation frames by decrementing. */
	ANIM_DIR_MAX,		/*!< The total amount of animation directions. */
} eAnimDirection;

// The default speed at which sprite actions occur in milliseconds.
static const int SPRITE_ACTION_SPEED_DEFAULT = 25; 

/*! \enum eSpriteAct
*	\brief Sprites can execute actions involving changing viewable size, color, and rotation.
*/
typedef enum eSpriteAct
{
	eSpriteActNone = 0x0000,			/*!< Sprite null action. Nothing occurs when this is active. */
	eSpriteActAnim = 0x0001,			/*!< Sprite animation action. */
	eSpriteActScale = 0x0002,			/*!< Sprite size changes to a given destination size. */
	eSpriteActColorPulse = 0x0004,		/*!< Sprite overlay color changes to a given destination color. */
	eSpriteActRotate = 0x0008,			/*!< Sprite rotates starting from the original angle, to the destination angle. */
	eSpriteActBlink = 0x0020,			/*!< Sprite blinks in and out of visibility. */
} eSpriteAct;

/*! \enum eSpriteAct
*	\brief Used to help identify what part or direction of the sprite action is currently occuring in.
*/
typedef enum eSpriteActState
{
	eSpriteActStateNone = 0x0000,		/*!< No action. */
	eSpriteActStatePulse1 = 0x0001,		/*!< General size change action state. */
	eSpriteActStatePulse2 = 0x0002,		/*!< General size change action state. */
	eSpriteActStateColPulse1 = 0x0004,	/*!< General color change action state. */
	eSpriteActStateColPulse2 = 0x0008,	/*!< General color change action state. */
	eSpriteActStateRotate1 = 0x0010,	/*!< General rotation action state. */
	eSpriteActStateRotate2 = 0x0020,	/*!< General rotation action state. */
	eSpriteActStateBlink1 = 0x0040,		/*!< General blink action state. */
	eSpriteActStateBlink2 = 0x0080,		/*!< General blink action state. */
	eSpriteActStateDone = 0x0100,		/*!< General action done state. */
} eSpriteActState;

/*! \enum eSpriteRotateDir
*	\brief Used to help initiate a sprite rotation action in a certain direction.
*/
typedef enum eSpriteRotateDir
{
	eSpriteRotNone = 0,			/*!< No rotation direction. */
	eSpriteRotClock,			/*!< Indicates that the sprite must rotate in a clockwise motion. */
	eSpriteRotCounterClock,		/*!< Indicates that the sprite must rotate in a counter-clockwise motion. */
	eSpriteRotMAX,				/*!< The total number of rotation directions available. */
} eSpriteRotateDir;



class CImage;

// The sprite class consists of an image, an animation sequence, and a gl texture.
/*! \class CSprite
* \brief The Sprite class.
*
* The Sprite class manages rendering of it's own #CImage, as well as run any animations that might be present.
*/
class CSprite
{
public:
	CImage* _images[SPRITE_ANIM_FRAMES_MAX];				/*!< If the sprite consists of an animation, then this is used to hold the animation image frames.  Otherwise, only the first frame is used. */
	BOOL _did_allocate_image_mem[SPRITE_ANIM_FRAMES_MAX];	/*!< Indicates that memory was allocated to load an image.  If not, free will not be called on the image. Otherwise, we know that we got our image from image loader which handles it's own free-ing. */
	GLuint _anim_texture_names[SPRITE_ANIM_FRAMES_MAX];		/*!< The gl texture names for the #_images. */
	int _x;													/*!< The x position relative to the position of the CGameObject that this sprite belongs to, not the world position. */
	int _y;													/*!< The y position relative to the position of the CGameObject that this sprite belongs to, not the world position. */
	int _z;													/*!< The z position relative to the position of the CGameObject that this sprite belongs to, not the world position. */
	int _num_anim_frames;									/*!< The total number of animation frames. */
	int _curr_anim_frame;									/*!< Holds the index into _images of the current frame of animation. */
	int _anim_frame_speeds_ms[SPRITE_ANIM_FRAMES_MAX];		/*!< The speed of each frame of animation in milliseconds. */
	int _anim_frame_time;									/*!< Used to keep track of the current anim frame time. */	
	eAnimDirection _anim_dir;								/*!< Indicates the direction that the animation frames are incremented or decremented. */
	float _anim_alpha;										/*!< Used to determine fade values when running a fade-type animation. This value is set to 0 when the current frame is fully visible. */
	color _color;				/*!< The color of the sprite. This is used to manipulate the color of the image(s) of the sprite. */
	Vector3 _scale;				/*!< The size scale for this sprite. 1.0 represents the actual size. */
	Vector3 _angle;				/*!< A rotation angle for this sprite. 0.0 represents the upright position. */
	Vector3 _orig_scale;			/*!< The starting size of the sprite, used for action calculations. */
	Vector3 _dest_scale;			/*!< The destination size that we want the sprite to reach during the pulse action. Used mostly for error correction that can happen when calculating the delta. */
	Vector3 _delta_scale;			/*!< The change in size over each speed tick. */
	color _orig_color;			/*!< The starting color of the sprite, used for action calculations. */
	color _dest_color;			/*!< The destination color that we want the sprite to reach during the color pulse action. Used mostly for error correction that can happen when calculating the delta. */
	color _delta_color;			/*!< The change in color over each speed tick. */
	Vector3 _orig_angle;			/*!< The starting angle of the sprite, used for action calculations. */
	Vector3 _dest_angle;			/*!< The destination angle that we want the sprite to reach during the rotate action. Used mostly for error correction that can happen when calculating the delta. */
	Vector3 _delta_angle;			/*!< The change in angle over each speed tick. */
	
private:
	
	int _largest_width;					/*!< The pixel width of the sprite's largest animation frame. */
	int _largest_height;				/*!< The pixel height of the sprite's largest animation frame. */
	int _largest_depth;					/*!< The pixel depth of the sprite's largest animation frame. */
	int _largest_half_width;			/*!< The pixel half-width of the sprite's largest animation frame. */
	int _largest_half_height;			/*!< The pixel half-height of the sprite's largest animation frame. */
	int _largest_half_depth;			/*!< The pixel half-depth of the sprite's largest animation frame. */	
	BOOL _is_visible;					/*!< Indicates whether this sprite will be rendered or not. */
	BOOL _is_animated;					/*!< Specifies that this sprite consists of frames of animation. */
	int _action_scale_time;				/*!< The duration that the action must be completed within. */
	int _action_scale_time_counter;		/*!< The time counter for the current action. */
	int _action_scale_num_pulses;		/*!< The number of times that the sprite will run a size scale action. One full size scale cycle consists of start to end size. */
	int _action_color_time;				/*!< The duration that the action must be completed within. */
	int _action_color_time_counter;		/*!< The time counter for the current action. */
	int _action_color_num_pulses;		/*!< The number of times that the color will pulse. Each pulse cycle is from the original color to the destination color. */
	int _action_rotate_time;			/*!< The duration that the action must be completed within. */
	int _action_rotate_time_counter;	/*!< The time counter for the current action. */
	int _action_rotate_num_rots;		/*!< The number of times that the sprite will rotate one full rotation. */
	int _action_blink_on_time;			/*!< The time that the sprite will be visible. */
	int _action_blink_on_time_rand;		/*!< The random factor for the sprite on-time visibility. This changes each action state transition. */
	int _action_blink_on_time_rand_val;	/*!< The random threshold value. _action_blink_on_time_rand is based off of this value. */ 
	int _action_blink_on_time_counter;	/*!< The time counter for the sprite on-time visibility. */
	int _action_blink_off_time;			/*!< The time that the sprite will be invisible. */
	int _action_blink_off_time_rand;	/*!< The random factor for the sprite off-time visibility. */
	int _action_blink_off_time_rand_val;/*!< The random threshold value. _action_blink_off_time_rand is based off of this value. */
	int _action_blink_off_time_counter;	/*!< The time counter for the sprite off-time visibility. */
	int _num_blinks;					/*!< The number of times that the sprite will blink in and out of visibiliy. One blink is one full cycle of on, then off. */
	int _action;						/*!< The sprite action. */
	int _action_state;					/*!< The sprite action state. */
	eSpriteRotateDir _rot_dir;			/*!< The sprite rotation direction. */
	eAnimType _anim_type;				/*!< Specifies what type of animation this sprite contaiins.  Look at the enum for a more detailed description. */
	
public:	
	/*! \fn CSprite()
	 *  \brief The CSprite class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CSprite();
	
	/*! \fn ~CSprite()
	 *  \brief The CSprite class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CSprite();
	
	/*! \fn init()
	 *  \brief The CSprite class initialization function.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void init(void);
	
	/*! \fn init(int x, int y, int z)
	 *  \brief The CSprite class initialization function with extra parameters.
	 *  
	 *	\param x The initial screen x location of the sprite.
	 *	\param y The initial screen y location of the sprite.
	 *	\param z The initial screen z location of the sprite. 0 by default.
	 *  \return n/a
	 */
	void init(int x, int y, int z = 0);
	
	/*! \fn init(int x, int y, int w, int h)
	 *  \brief The CSprite class initialization function with extra parameters.
	 *  
	 *	\param x The initial screen x location of the sprite.
	 *	\param y The initial screen y location of the sprite.
	 *	\param w The width of the sprite.
	 *	\param y The height of the sprite.
	 *  \return n/a
	 */
	void init(int x, int y, int w, int h);
	
	/*! \fn loadSpriteImage(int imageFileID)
	 *  \brief Given an image ID, loads the image from the disk into the sprite.
	 *  
	 *	\param imageFileID The image ID of the image file.
	 *  \return n/a
	 */
	void loadSpriteImage(int imageFileID);
	
	/*! \fn loadSpriteImage(char* fileName)
	 *  \brief Given an image filename, creates a new image and loads it into the sprite.
	 *  
	 *	\param fileName The file name of the image.
	 *  \return n/a
	 */
	void loadSpriteImage(char* fileName);
	
	/*! \fn loadSpriteImage(CImage* image)
	 *  \brief Sets the sprite's image to an already loaded image.
	 *  
	 *	\param image A pointer the the already loaded #CImage.
	 *  \return n/a
	 */
	void loadSpriteImage(CImage* image);
	
	/*! \fn bindSpriteImage(uint32 imageID)
	 *  \brief If the sprite has already been loaded but no image has been assiciated with it, this can be called to bind an already loaded image to a texture name for rendering.
	 *  
	 *	\param imageID The image ID to bind to this sprite.
	 *  \return n/a
	 */
	void bindSpriteImage(uint32 imageID);
	
	/*! \fn getImage()
	 *  \brief Gets the current active image in the sprite.
	 *  
	 *	\param n/a
	 *  \return A pointer to the current active image.
	 */
	inline CImage* getImage() const {return _images[_curr_anim_frame];}
	
	/*! \fn getTexName()
	 *  \brief Gets the texture name of the current active image in the sprite.
	 *  
	 *	\param n/a
	 *  \return The texture name of the current active image.
	 */
	inline GLuint getTexName() const {return _anim_texture_names[_curr_anim_frame];}
	
	/*! \fn isVisible()
	 *  \brief Returns a value that indicates whether this sprite is visible or not.
	 *  
	 *	\param n/a
	 *  \return TRUE if this sprite is visible, FALSE otherwise.
	 */
	inline BOOL isVisible() const {return _is_visible;}
	
	/*! \fn destroy(void)
	 *  \brief The CSprite class destroy function.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void destroy(void);
	
	/*! \fn draw(void)
	 *  \brief The CSprite class render function.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void draw(void);
	
	/*! \fn draw(float x, float y)
	 *  \brief The CSprite class render function with extra parameters.
	 *  
	 *	\param x The screen x location to render the sprite.
 	 *	\param y The screen y location to render the sprite.
	 *  \return n/a
	 */
	void draw(float x, float y);
	
	/*! \fn drawCentered(void)
	 *  \brief Renders the sprite to the screen using the center as the anchor point.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void drawCentered(void);
	
	/*! \fn drawCentered(float x, float y)
	 *  \brief Renders the sprite to the screen using the center as the anchor point.
	 *  
	 *	\param x The screen x location to render the sprite.
 	 *	\param y The screen y location to render the sprite.
	 *  \return n/a
	 */
	void drawCentered(float x, float y);
	
	/*! \fn getWidth()
	 *  \brief Gets the width of the sprite's current active animation frame.
	 *  
	 *	\param n/a
	 *  \return The width of this sprite.
	 */
	int getWidth() const;
	
	/*! \fn getHeight()
	 *  \brief Gets the height of the sprite's current active animation frame.
	 *  
	 *	\param n/a
	 *  \return The height of this sprite.
	 */
	int getHeight() const;
	
	/*! \fn getDepth()
	 *  \brief Gets the depth of the sprite's current active animation frame.
	 *  
	 *	\param n/a
	 *  \return The depth of this sprite.
	 */
	int getDepth() const;
	
	/*! \fn getLargestWidth()
	 *  \brief Gets the largest width of all the sprite's animation frames.
	 *  
	 *	\param n/a
	 *  \return The largest width of this sprite.
	 */
	inline int getLargestWidth() { return _largest_width; }
	
	/*! \fn getLargestHeight()
	 *  \brief Gets the largest height of all the sprite's animation frames.
	 *  
	 *	\param n/a
	 *  \return The largest height of this sprite.
	 */
	inline int getLargestHeight() { return _largest_height; }
	
	/*! \fn getLargestDepth()
	 *  \brief Gets the largest depth of all the sprite's animation frames.
	 *  
	 *	\param n/a
	 *  \return The largest depth of this sprite.
	 */
	inline int getLargestDepth() { return _largest_depth; }
	
	/*! \fn getLargestHalfWidth()
	 *  \brief Gets the largest half-width of all the sprite's animation frames.
	 *  
	 *	\param n/a
	 *  \return The largest half-width of this sprite.
	 */
	inline int getLargestHalfWidth() { return _largest_half_width; }
	
	/*! \fn getLargestHalfHeight()
	 *  \brief Gets the largest half-height of all the sprite's animation frames.
	 *  
	 *	\param n/a
	 *  \return The largest half-height of this sprite.
	 */
	inline int getLargestHalfHeight() { return _largest_half_height; }
	
	/*! \fn getLargestHalfDepth()
	 *  \brief Gets the largest half-depth of all the sprite's animation frames.
	 *  
	 *	\param n/a
	 *  \return The largest half-depth of this sprite.
	 */
	inline int getLargestHalfDepth() { return _largest_half_depth; }	
	
	/*! \fn getHalfWidth()
	 *  \brief Gets the half-width of the sprite's current active animation frame.
	 *  
	 *	\param n/a
	 *  \return The half-width of this sprite.
	 */
	int getHalfWidth() const;
	
	/*! \fn getHalfHeight()
	 *  \brief Gets the half-height of the sprite's current active animation frame.
	 *  
	 *	\param n/a
	 *  \return The half-height of this sprite.
	 */
	int getHalfHeight() const;
	
	/*! \fn getHalfDepth()
	 *  \brief Gets the half-depth of this sprite.
	 *  
	 *	\param n/a
	 *  \return The half-depth of this sprite.
	 */
	int getHalfDepth() const;
	
	/*! \fn updateAnim()
	 *  \brief The animation update function.
	 *  
	 * This updates the animation using #TIME_LAST_FRAME.
	 *	\param n/a
	 *  \return n/a
	 */
	void updateAnim(void);
	
	/*! \fn setFrameSpeedsMS(int speed)
	 *  \brief Sets all frame speeds to one uniform speed in milliseconds.
	 *  
	 *	\param speed The speed in milliseconds for the animation.
	 *  \return n/a
	 */
	void setFrameSpeedsMS(int speed);
	
	/*! \fn resetAnim(void)
	 *  \brief Set the current frame of animation back to zero.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	inline void resetAnim(void) {_curr_anim_frame = 0;}
	
	/*! \fn setCurrAnimFrame(int frame)
	 *  \brief Set the current frame of animation back to zero.
	 *  
	 *	\param frame The frame number.
	 *  \return n/a
	 */
	void setCurrAnimFrame(int frame);
	
	/*! \fn setSizeScaleAction(float destScaleX, float destScaleY, float destScaleZ, int time, int numPulses = __INF)
	 *  \brief Initiates the sprite action that scales the viewable size of the sprite to the given destination scale.
	 *  
	 *	\param destScaleX The x scale that this sprite will scale to.
	 *	\param destScaleY The y scale that this sprite will scale to.
	 *	\param destScaleZ The z scale that this sprite will scale to.
	 *	\param time The amount time that this action runs for.
	 *	\param numPulses The number of times that the size is pulsed.
	 *  \return n/a
	 */
	void setSizeScaleAction(float destScaleX, float destScaleY, float destScaleZ, int time, int numPulses = __INF);
	
	/*! \fn setSizeScaleAction(Vector3 destScale, int time, int numPulses = __INF)
	 *  \brief Initiates the sprite action that scales the viewable size of the sprite to the given destination scale.
	 *  
	 *	\param destScale The scale that this sprite will scale to.
	 *	\param time The amount time that this action runs for.
	 *	\param numPulses The number of times that the size is pulsed.
	 *  \return n/a
	 */
	void setSizeScaleAction(Vector3 destScale, int time, int numPulses = __INF);	
	
	/*! \fn setColorPulseAction(float r, float g, float b, float a, int time, int numPulses)
	 *  \brief Initiates the sprite action that changes the sprite color to the given destination color.
	 *  
	 *	\param r The red color that this sprite will change to.
	 *	\param g The green color that this sprite will change to.
	 *	\param b The blue color that this sprite will change to.
	 *	\param a The alpha value that this sprite will change to.
	 *	\param time The amount time that this action runs for.
	 *  \return n/a
	 */
	void setColorPulseAction(float r, float g, float b, float a, int time, int numPulses = __INF);
	
	/*! \fn setColorPulseAction(color destColor, int time, int numPulses = __INF)
	 *  \brief Initiates the sprite action that changes the sprite color to the given destination color.
	 *  
	 *	\param destColor The color that this sprite will change to.
	 *	\param time The amount time that this action runs for.
	 *	\param numPulses The number of times that the color is pulsed.
	 *  \return n/a
	 */
	void setColorPulseAction(color destColor, int time, int numPulses = __INF);
	
	/*! \fn setRotateAction(Vector3 destAngle, eSpriteRotateDir direction, int time = 1000, int numRots = __INF)
	 *  \brief Initiates the sprite action that rotates the sprite to a given destination angle.
	 *  
	 * The time value is ignored when the eSpriteActRotateContinuous is passed in. This is because of the fact that the sprite will just continuously rotate until it is explicitly told to stop.
	 *
	 *	\param destAngle The angle that this sprite will rotate to.
	 *	\param direction The direction that this sprite will rotate.
	 *	\param time The amount time that this action runs for.
	 *	\param numRots The number of times that the sprite is rotated.
	 *  \return n/a
	 */
	void setRotateAction(Vector3 destAngle, eSpriteRotateDir direction, int time = 1000, int numRots = __INF);
	
	/*! \fn setBlinkAction(int onTime, int randOnTime, int offTime, int randOffTime, int numBlinks)
	 *  \brief Initiates the sprite action that causes the sprite to blink in and out of visibility.
	 *  
	 *	\param action The specific rotate action that this sprite will run.
	 *	\param onTime The amount of time that the sprite stays visible during the duration of this action.
	 *	\param randOnTime The randomness threshold for the on-time. This value may be zero for no randomness.
	 *	\param offTime The amount of time that the sprite stays invisible during the duration of this action.
	 *	\param randOffTime The randomness threshold for the off-time. This value may be zero for no randomness.
	 *	\param numBlinks The number of blink cycles that this action will execute.
	 *  \return n/a
	 */
	void setBlinkAction(int onTime, int randOnTime, int offTime, int randOffTime, int numBlinks = __INF);
	
	/*! \fn updateAction()
	 *  \brief The sprite action update function.
	 *  
	 * This updates the sprite action(s) using #TIME_LAST_FRAME.
	 *	\param n/a
	 *  \return n/a
	 */
	void updateAction(void);
	
	/*! \fn resetAction(void)
	 *  \brief Set the current action back to null.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	inline void resetAction(void) { _action = 0; _action_state = 0; }
	
	/*! \fn setVisible(BOOL visible)
	 *  \brief Set the visibility of the sprite.
	 *  
	 *	\param visible TRUE for visible, FALSE otherwise.
	 *  \return n/a
	 */
	inline void setVisible(BOOL visible) { _is_visible = visible; }

	/*! \fn setAnimType(eAnimType type)
	 *  \brief Set the animation type of the sprite.
	 *  
	 *	\param type The animation type, one of the enum eAnimType.
	 *  \return n/a
	 */
	inline void setAnimType(eAnimType type) { _anim_type = type; }

	/*! \fn getAnimType(void)
	 *  \brief Returns the current animation type, of type eAnimType.
	 *  
	 *	\param n/a
	 *  \return The animation type of type eAnimType.
	 */
	inline eAnimType getAnimType(void) { return _anim_type; }

	/*! \fn setAnimDirection(eAnimDirection dir)
	 *  \brief Set the animation direction type of the sprite.
	 *  
	 *	\param type The animation direction type, one of the enum eAnimDirection.
	 *  \return n/a
	 */
	inline void setAnimDirection(eAnimDirection dir) { _anim_dir = dir; }
	
	inline int getCurrentNumColorPulses(void) { return _action_color_num_pulses; }
	inline void setCurrentNumColorPulses(int numPulses) { _action_color_num_pulses = numPulses; }
	inline void setColor(float r, float g, float b) { _color.r = r; _color.g = g; _color.b = b; }
	inline void setColor(float r, float g, float b, float a) { _color.r = r; _color.g = g; _color.b = b; _color.a = a; }
	inline void setColor(const color theColor) { _color = theColor; }
	inline const color getColor(void) { return _color; }
	inline void setAlpha(float a) { _color.a = a; }
};

#endif