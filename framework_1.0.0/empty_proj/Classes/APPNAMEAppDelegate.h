#import <UIKit/UIKit.h>
#import "Engine.h"
//#import "TouchSystem.h"

#define FRAME_INTERVAL	(1.0/60.0)


@class EAGLView;
@class APPNAMEAppDelegate;

@interface APPNAMEAppDelegate : NSObject
{
	EAGLView* _glView;
	
	// The touch area is the entire window.
	//TouchSystem* _window;
	UIWindow* _window;
    BOOL openFeintInitialized;
}
@property (nonatomic) BOOL openFeintInitialized;

- (int) getCurrentTimeMillis;
+ (APPNAMEAppDelegate*) getInstance;
- (void) setLandscapeMode;
- (void) setPortraitMode;
- (EAGLView*) getView;
- (void)initializeFlurry;


@end

