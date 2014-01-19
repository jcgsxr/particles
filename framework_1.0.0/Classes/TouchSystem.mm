#include "types.h"
#include "AppDefines.h"
#import "TouchSystem.h"
#import "Engine.h"
#import "SystemDefines.h"

#if defined (ENABLE_TOUCH_SYSTEM)

@implementation TouchSystem

CGPoint startTouchPosition;
eSwipeDirection currSwipeDir = SWIPE_NONE;

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) 
	{
        // Initialization code here.
		//_owner = owner;
    }
    return self;
}


// Handle touch events
- (void)touchesChangedWithEvent:(UIEvent *)event
{	
	// Iterate through all the touches currently active
	NSSet *touches = [event allTouches];	
	
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


- (void)drawRect:(CGRect)rect 
{
    // Drawing code here.
}

- (void)dealloc
{
	[super dealloc];
}

@end


#else

@implementation TouchSystem


// Handle touch events
- (void)touchesChangedWithEvent:(UIEvent *)event
{	

}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{

}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{

}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	
}


- (void)drawRect:(CGRect)rect 
{

}

- (void)dealloc
{
	[super dealloc];
}

@end

#endif