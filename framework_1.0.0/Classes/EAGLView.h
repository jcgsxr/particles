#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/EAGLDrawable.h>


/*! \interface EAGLView
 * \brief The EAGL interface.
 *
 * OpenGL properties are initialized within this class.
 */
@interface EAGLView : UIView
{
	
@private
	GLint backingWidth;		/*!< The pixel width of the backbuffer. */
	GLint backingHeight;	/*!< The pixel height of the backbuffer. */
	EAGLContext *context;	/*!< An EAGLContext object manages the state information, commands, and resources needed to draw using OpenGL ES. All OpenGL ES commands are executed in relation to an EAGL context. */
	GLuint viewRenderbuffer;	/*!< OpenGL name for the renderbuffer used to render to this view */
	GLuint viewFramebuffer;		/*!< OpenGL name for the framebuffer used to render to this view */
	GLuint depthRenderbuffer;	/*!< OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
}

/*! \fn swapBuffers
 *  \brief Renders the frame buffer to the screen.
 *  
 *	\param n/a
 *  \return n/a
 */
- (void)swapBuffers;

/*! \fn setBuffers
 *  \brief Sets the context to which the frame buffer will render to.
 *  
 *	\param n/a
 *  \return n/a
 */
- (void)setBuffers;


@end