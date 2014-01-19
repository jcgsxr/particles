#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "types.h"
#include "Sprite.h"
#include "physics_types.h"

#define NUM_GO_SPRITES_MAX	8

/*! \class CGameObject
 * \brief The Game Object class.
 *
 * A game object exists as an entity in the game world, may contain multiple sprites, and may have physics associated with it.
 * The game object is used when there is a need to render sprite(s) and also when there is a need to be able to run physics on it.
 */
class CGameObject
{
public:
	int _num_sprites;						/*!< The number of active sprites associated with this game object. */
	CSprite _sprites[NUM_GO_SPRITES_MAX];	/*!< The sprite array. */
	physicsObject _phys;					/*!< The physics object that represents this gameobject's physical properties in the world. */
	float _width;							/*!< Since the game object can consist of more than one sprite and also a physics object, this holds the general width of the object, usually the width of the largest sprite associated with the game object. */
	float _height;							/*!< Since the game object can consist of more than one sprite and also a physics object, this holds the general height of the object, usually the height of the largest sprite associated with the game object. */
	BOOL _stop_rot_on_vel_zero;				/*!< When set to TRUE, rotation is set to zero when the velocity is zeroed out or is zero. */
	float _alpha;							/*!< This represents the alpha for all sprites belonging to this game object. */
	BOOL _should_wrap_x;					/*!< Indicates whether the game object should wrap around the screen horizontal wise. */
	BOOL _should_wrap_y;					/*!< Indicates whether the game object should wrap around the screen vertical wise. */
	BOOL _is_active;						/*!< Indicates whether to render and update the game object. */
	
	/*! \fn CGameObject()
	 *  \brief The CGameObject class constructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	CGameObject();
	
	/*! \fn ~CGameObject()
	 *  \brief The CGameObject class destructor.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	~CGameObject();
	
	/*! \fn init(float x, float y)
	 *  \brief The CGameObject class initialization function.
	 *  
	 * Game object properties are initialized here.
	 *	\param x The x location of the game object.
	 *	\param y The y location of the game object.
	 *  \return n/a
	 */
	void init(float x, float y);
	
	/*! \fn init()
	 *  \brief The CGameObject class initialization function.
	 *  
	 * Game object properties are initialized here.
	 *	\param n/a
	 *  \return n/a
	 */
	void init(void);
	
	/*! \fn addSprite(const CSprite& sprite)
	 *  \brief Adds a sprite to the game object's sprite array.
	 *
	 * The sprite being passed in must already be initialized.
	 *	\param sprite The sprite to be added to the game object's sprite array.
	 *  \return n/a
	 */
	void addSprite(const CSprite& sprite);
	
	/*! \fn loadImageIntoSprite(CImage* image)
	 *  \brief Adds a sprite to the game object's sprite array.
	 *  
	 * A CSprite will be initialized before being added to the sprite array.
	 *	\param image The image that is loaded into the sprite, which is then added to the game object's sprite array.
	 *  \return n/a
	 */
	void loadImageIntoSprite(CImage* image);
	
	/*! \fn destroy()
	 *  \brief The CGameObject class destroy function.
	 *  
	 * Game object properties are deallocated here.
	 *	\param n/a
	 *  \return n/a
	 */
	void destroy(void);
	
	/*! \fn draw(int x, int y)
	 *  \brief The CGameObject class draw function.
	 *  
	 * All of the game object's sprites within the sprite array are rendered here.
	 *	\param x The x location to render the game object.
	 *	\param y The y location to render the game object.
	 *  \return n/a
	 */
	void draw(int x, int y);
	
	/*! \fn draw()
	 *  \brief The CGameObject class draw function.
	 *  
	 * All of the game object's sprites within the sprite array are rendered here.
	 *	\param n/a
	 *  \return n/a
	 */
	void draw();
	
	/*! \fn setXWrap(BOOL should_wrap)
	 *  \brief Set whether the game object will wrap around the screen horizonal-wise.
	 *  
	 *	\param should_wrap Boolean indicating whether the game object should wrap around the screen.
	 *  \return n/a
	 */
	void setXWrap(BOOL should_wrap) {_should_wrap_x = should_wrap;};
	
	/*! \fn setYWrap(BOOL should_wrap)
	 *  \brief Set whether the game object will wrap around the screen vertical-wise.
	 *  
	 *	\param should_wrap Boolean indicating whether the game object should wrap around the screen.
	 *  \return n/a
	 */
	void setYWrap(BOOL should_wrap) {_should_wrap_y = should_wrap;};
	
	/*! \fn setAlpha(float alpha)
	 *  \brief Sets the alpha values for all sprites belonging to the game object.
	 *  
	 *	\param alpha The alpha value. 0.0 indicates fully transparent, 1.0 indicates fully opaque.
	 *  \return n/a
	 */
	void setAlpha(float alpha);
	
	/*! \fn setDimensions(float w, float h)
	 *  \brief Sets the pixel width and height of the game object.
	 *  
	 *	\param w The pixel width of the game object.
	 *	\param h The pixel height of the game object.
	 *  \return n/a
	 */
	void setDimensions(float w, float h);
	
	/*! \fn setDimAsLargestSpriteDim()
	 *  \brief Sets the pixel width and height of the game object to the largest sprite in the sprite array.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	void setDimAsLargestSpriteDim(void);
	
	/*! \fn setPosition(float x, float y)
	 *  \brief Sets the game object's physics object's position.
	 *  
	 *	\param x The x position of the game object's physics object #_phys. 
	 *	\param y The y position of the game object's physics object #_phys.
	 *  \return n/a
	 */
	void setPosition(float x, float y);
	
	/*! \fn setVelocity(float x, float y)
	 *  \brief Sets the game object's physics object's velocity.
	 *  
	 *	\param x The x velocity of the game object's physics object #_phys.
	 *	\param y The y velocity of the game object's physics object #_phys.
	 *  \return n/a
	 */
	void setVelocity(float x, float y);
	
	/*! \fn update()
	 *  \brief The CGameObject class update function.
	 *  
	 * Physics and screen wrapping, if enabled, are updated here.
	 *	\param n/a
	 *  \return n/a
	 */
	void update(void);
};


#endif
