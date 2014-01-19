#import "EAGLView.h"
#import <sys/time.h>
#import "types.h"
#import "engine.h"
#import "GameData.h"
#import "SystemDefines.h"
#import "MathUtil.h"
#import "AppDefines.h"
#import "particlesAppDelegate.h"
#import "SettingsController.h"



#if defined (LANDSCAPE_MODE)
int SCRN_W = 480;
int SCRN_H = 320;
int SCRN_D = 100;
int HALF_SCRN_W = 240;
int HALF_SCRN_H = 160;
int HALF_SCRN_D = 50;
#else
int SCRN_W = 320;
int SCRN_H = 480;
int SCRN_D = 100;
int HALF_SCRN_W = 160;
int HALF_SCRN_H = 240;
int HALF_SCRN_D = 50;
//int SCRN_W = 640;
//int SCRN_H = 960;
//int SCRN_D = 100;
//int HALF_SCRN_W = 320;
//int HALF_SCRN_H = 480;
//int HALF_SCRN_D = 50;
#endif


particlesAppDelegate* delegateSingleton = NULL;

@implementation particlesAppDelegate


- (void)applicationDidFinishLaunching:(UIApplication *)application 
{   
	// set the delegate handle
	if(delegateSingleton == NULL)
	{
		delegateSingleton = self;
	}
    
	//CGRect rect = [[UIScreen mainScreen] bounds];
	CGRect rect = CGRectMake(0, 0, SCRN_W, SCRN_H);
	
	//Create the OpenGL view and add it to the window
	_glView = [[EAGLView alloc] initWithFrame:rect];
	
	//_window = [[TouchSystem alloc] initWithFrame: rect];
	_window = [[UIWindow alloc] initWithFrame:rect];

#if defined (LANDSCAPE_MODE)
	[self setLandscapeMode];
#else
	[self setPortraitMode];
#endif
	
	initApp();
	
	//Swap framebuffer
	[_glView swapBuffers];
	
	[_window addSubview:_glView];
	[_window addSubview:_settingsCtrl.view];
    
	//Show the window
	[_window makeKeyAndVisible];
	
	#if defined (LANDSCAPE_MODE)
	// Set orientation to landscape.
	[[UIDevice currentDevice] setOrientation:UIInterfaceOrientationLandscapeRight];
	// Prevents uialertviews from displaying in portrait orientation (appirater).
	[application setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
	#endif
	
    //[self initializeFlurry];
    
	// Create an NSTimer and have it call main's handler functions to do update and render every loop.
	[NSTimer scheduledTimerWithTimeInterval:((FRAME_INTERVAL)) target:self selector:@selector(runLoop) userInfo:nil repeats:TRUE];
		
	// Clear the frame buffer to erase any nasties.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}



- (void)runLoop
{
	TIME_LAST_FRAME = [self getCurrentTimeMillis] - FRAME_START_TIME;
	
	FRAME_START_TIME = [self getCurrentTimeMillis];

#if ENABLE_3D
	// Clear the drawing buffer.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
#endif
	
	updateApp();

	//Swap framebuffer
	[_glView swapBuffers];
}


- (int) getCurrentTimeMillis
{
	// Calculate time last frame.
	gettimeofday(&CURRENT_TIME, NULL);
	
	TIMEVAL_LAST_FRAME.tv_sec = CURRENT_TIME.tv_sec - TEMP_TIME_VAL.tv_sec;
	TIMEVAL_LAST_FRAME.tv_usec = CURRENT_TIME.tv_usec - TEMP_TIME_VAL.tv_usec;
	
	// Do some math to correct the microsecond value if it turns out to be negative.
	if (TIMEVAL_LAST_FRAME.tv_usec < 0)
	{
		// Basically, just take the value of 1 microsecond and add the negative value
		// which then yields the correct microsecond value.
		TIMEVAL_LAST_FRAME.tv_usec = 1000000 + TIMEVAL_LAST_FRAME.tv_usec;
		// Make sure to update the seconds value as well.
		--TIMEVAL_LAST_FRAME.tv_sec;
	}
	
	gettimeofday(&TEMP_TIME_VAL, NULL);
	
	// Now grab the time in milliseconds since we don't need such high precision.
	return (((TIMEVAL_LAST_FRAME.tv_sec * 1000000) + TIMEVAL_LAST_FRAME.tv_usec) / 1000);
}

- (void)dealloc 
{
	destroyApp();
    [_window release];
	[_glView release];
	[_settingsCtrl release];
    [super dealloc];
}


+ (particlesAppDelegate*) getInstance
{
	return delegateSingleton;
}


- (void) setLandscapeMode
{
	// Do some extra work to set up landscape view.
	// Notice the swap of width and height to get the new center point.
	_glView.center = CGPointMake((SCRN_H / 2), (SCRN_W / 2));
	CGAffineTransform transform = CGAffineTransformIdentity;
	// Now apply a 90 defgree rotation around the new center point.
	transform = CGAffineTransformRotate(transform, DEGREES_TO_RADIANS(90));
	//_glView.transform = transform;
	[_glView setTransform:transform];
	
	// JC: This is a nasty way to make touch work... but it works.
	_window.center = CGPointMake((SCRN_H / 2), (SCRN_W / 2));
	_window.bounds = CGRectMake(0, 0, SCRN_H, SCRN_W);
}


- (void) setPortraitMode
{
	_glView.center = CGPointMake((SCRN_W / 2), (SCRN_H / 2));
	CGAffineTransform transform = CGAffineTransformIdentity;
	//transform = CGAffineTransformRotate(transform, DEGREES_TO_RADIANS(0));
	//_glView.transform = transform;
	[_glView setTransform:transform];
	
	// JC: This is a nasty way to make touch work... but it works.
	_window.center = CGPointMake((SCRN_W / 2), (SCRN_H / 2));
	_window.bounds = CGRectMake(0, 0, SCRN_W, SCRN_H);
}


- (EAGLView*) getView
{
	return _glView;
}



@end