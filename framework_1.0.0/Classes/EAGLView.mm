#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"
#import "types.h"
#import "Engine.h"

#define USE_DEPTH_BUFFER 0

// A class extension to declare private methods
@interface EAGLView ()

@property (nonatomic, retain) EAGLContext *context;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end


@implementation EAGLView 

CGPoint startTouchPosition;
eSwipeDirection currSwipeDir = SWIPE_NONE;

@synthesize context;

// You must implement this
+ (Class) layerClass 
{
	return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder 
{
	context = NULL;

	if ((self = [super initWithCoder:coder])) {
		// Get the layer
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
		
		eaglLayer.opaque = YES;
		eaglLayer.drawableProperties = @{ kEAGLDrawablePropertyRetainedBacking : @(NO), kEAGLDrawablePropertyColorFormat : kEAGLColorFormatRGBA8 };
//		[NSDictionary dictionaryWithObjectsAndKeys:
//		   [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		
		if (!context || ![EAGLContext setCurrentContext:context] || ![self createFramebuffer]) 
		{
			[self release];
			return nil;
		}
	}
	return self;
}


- (id)initWithFrame:(CGRect)frame 
{
	if(self = [super initWithFrame:frame])
	{
		context = NULL;
		
		// Set bounds.
		//self.bounds = frame;
		
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
		
		eaglLayer.opaque = YES;

		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		
		if (!context || ![EAGLContext setCurrentContext:context] || ![self createFramebuffer]) 
		{
			[self release];
			return nil;
		}
		
	}
	return self;	
}

- (void)layoutSubviews 
{
	[EAGLContext setCurrentContext:context];
	[self destroyFramebuffer];
	[self createFramebuffer];
}


- (BOOL)createFramebuffer 
{
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	//glViewport(0, 0, backingWidth, backingHeight);
	
	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) 
	{
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}
	
	return YES;
}


- (void)destroyFramebuffer 
{
	
	glDeleteFramebuffersOES(1, &viewFramebuffer);
	viewFramebuffer = 0;
	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;
	
	if(depthRenderbuffer) {
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
}


- (void) setBuffers
{
	[EAGLContext setCurrentContext:context];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);	
}


- (void) swapBuffers
{
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
}




- (eTouchPhase) getTouchPhase:(UITouchPhase)touchPhase
{
	switch (touchPhase)
	{
		case UITouchPhaseBegan:
			return TouchPhaseBegan;
		case UITouchPhaseMoved:
			return TouchPhaseMoved;
		case UITouchPhaseEnded:
			return TouchPhaseEnded;
	}
	
	return TouchPhaseCanceled;
}


// Handle touch events
- (void)touchesChangedWithEvent:(UIEvent *)event
{	
	// Iterate through all the touches currently active
	NSSet *touches = [event allTouches];
	
	switch ([touches count])
	{
		case 1:
		{	
			for (UITouch *touch in touches) 
			{
				CGPoint location = [touch locationInView:self];
				
				// Convert the touch's phase to one of ours
				eTouchPhase touchPhase;
				switch (touch.phase) 
				{
					case UITouchPhaseBegan:
					{
						// A swipe may have started
						startTouchPosition = [touch locationInView:self]; 
						touchPhase = TouchPhaseBegan;
						
					}
						break;
						
					case UITouchPhaseMoved:
					{
						
						int dx = fabs(startTouchPosition.x - location.x);
						int dy = fabs(startTouchPosition.y - location.y);
						
						if(dx > dy)
						{
							// diagonal swipes count as horizontal.
							if(location.x < startTouchPosition.x - HORIZ_SWIPE_DRAG_MIN)
							{
								currSwipeDir = SWIPE_LEFT;
							}
							else if(location.x > startTouchPosition.x + HORIZ_SWIPE_DRAG_MIN)
							{
								currSwipeDir = SWIPE_RIGHT;
							}
						}
						else
						{
							// If the current location is to the left of the start, its a swipe left, etc
							if(location.y < startTouchPosition.y - VERT_SWIPE_DRAG_MIN)
							{
								currSwipeDir = SWIPE_UP;
							}
							else if(location.y > startTouchPosition.y + VERT_SWIPE_DRAG_MIN)
							{
								currSwipeDir = SWIPE_DOWN;
							}
						}				
						
						touchPhase = TouchPhaseMoved;
					}
						break;
						
					case UITouchPhaseEnded:
					{
						touchPhase = TouchPhaseEnded;
					}
						break;
						
					//case UITouchPhaseCanceled:
					//{
					//startTouchPosition = [touch locationInView:self];
					//	touchPhase = TouchPhaseCanceled;
					//}
					//break;
						
					default:
						break;
				}
				
				engine->touchEventHandler(location.x, location.y, touchPhase, currSwipeDir);
			}
		}
			break;
			
		case 2:
		default:
		{
			UITouch *touch1 = [[touches allObjects] objectAtIndex:0];
			CGPoint location1 = [touch1 locationInView:self];
			UITouch *touch2 = [[touches allObjects] objectAtIndex:1];
			CGPoint location2 = [touch2 locationInView:self];
			
			eTouchPhase touchPhase1 = [self getTouchPhase:touch1.phase];
			eTouchPhase touchPhase2 = [self getTouchPhase:touch2.phase];
			
			engine->multiTouchEventHandler(
										   location1.x, 
										   location1.y, 
										   touchPhase1, 
										   location2.x,
										   location2.y,
										   touchPhase2,
										   currSwipeDir);	
		}
			break;
	}
}



- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self touchesChangedWithEvent:event];
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self touchesChangedWithEvent:event];
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[self touchesChangedWithEvent:event];
}

- (void)dealloc 
{
	
	//[self stopAnimation];
	
	if ([EAGLContext currentContext] == context) {
		[EAGLContext setCurrentContext:nil];
	}
	
	[context release];	
	[super dealloc];
}

@end
