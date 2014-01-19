#ifndef __BASICINTERFACE_H__
#define __BASICINTERFACE_H__

#include "types.h"

/*! \class CBasicInterface
 * \brief The Basic Interface class.
 *
 * CEngine relies on the fact that all screens implement this interface. All screens must implement init(), destroy(), update(), draw(), and handleTouch().
 */
class CBasicInterface
{
public:
	
	/*! \fn init(void)
	 *  \brief The initialize function that all screens must implement. Screen properties should be organized in a way that they can be re-initialized at any time without any issues.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	virtual void init(void) = 0;
	
	/*! \fn destroy(void)
	 *  \brief The destroy function that all screens must implement. Screen properties should be organized in a way that they can be deallocated at any time without any issues.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	virtual void destroy(void) = 0;
	
	/*! \fn update(void)
	 *  \brief The update function that all screens must implement. General game logic is placed here.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */
	virtual void update(void) = 0;
	
	/*! \fn draw(void)
	 *  \brief The draw function that all screens must implement. Rendering occurs here.
	 *  
	 *	\param n/a
	 *  \return n/a
	 */	
	virtual void draw(void) = 0;
	
	/*! \fn handleTouch(float x, float y, eTouchPhase phase)
	 *  \brief The touch handling function that all screens must implement.
	 *  
	 *	\param x The x position on the screen where the touch is occuring.
 	 *	\param y The y position on the screen where the touch is occuring.
 	 *	\param phase Indicates the state of the touch.
	 *  \return n/a
	 */
	virtual void handleTouch(float x, float y, eTouchPhase phase) = 0;
	
	/*! \fn handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2)
	 *  \brief The touch handling function that all screens must implement.
	 *  
	 *	\param x1 The x position on the screen where the first touch is occuring.
 	 *	\param y1 The y position on the screen where the first touch is occuring.
  	 *	\param phase1 Indicates the state of the touch of the first touch.
	 *	\param x2 The x position on the screen where the second touch is occuring.
 	 *	\param y2 The y position on the screen where the second touch is occuring.
 	 *	\param phase2 Indicates the state of the touch of the second touch.
	 *  \return n/a
	 */
	virtual void handleMultiTouch(float x1, float y1, eTouchPhase phase1, float x2, float y2, eTouchPhase phase2) = 0;
	
	//virtual void suspend(void) = 0;
	//virtual void resume(void) = 0;
	
};


#endif