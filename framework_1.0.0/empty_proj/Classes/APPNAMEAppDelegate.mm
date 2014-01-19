#import "EAGLView.h"
#import <sys/time.h>
#import "types.h"
#import "engine.h"
#import "GameData.h"
#import "SystemDefines.h"
#import "Appirater.h"
#import "FlurryAPI.h"
#import "MathUtil.h"
#import "AppDefines.h"
#import "APPNAMEAppDelegate.h"

#if defined (ENABLE_OPENFEINT)
#import "OpenFeint.h"
#endif

APPNAMEAppDelegate* delegateSingleton = NULL;

@implementation APPNAMEAppDelegate

@synthesize openFeintInitialized;

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
    
	//Show the window
	[_window makeKeyAndVisible];
	
	#if defined (LANDSCAPE_MODE)
	// Set orientation to landscape.
	[[UIDevice currentDevice] setOrientation:UIInterfaceOrientationLandscapeRight];
	// Prevents uialertviews from displaying in portrait orientation (appirater).
	[application setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
	#endif
	
    [self initializeFlurry];
    
    [Appirater appLaunched:355801939 daysUntilPrompt:10 launchesUntilPrompt:15 isLandscape:YES debug:NO];
    
	// Create an NSTimer and have it call main's IPL*_handler functions to do update and render every loop.
	[NSTimer scheduledTimerWithTimeInterval:((FRAME_INTERVAL)) target:self selector:@selector(runLoop) userInfo:nil repeats:TRUE];
		
	// Clear the frame buffer to erase any nasties.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


void uncaughtExceptionHandler(NSException *exception) 
{
    [FlurryAPI logError:@"Uncaught" message:@"Crash!" exception:exception];
}


- (void)initializeFlurry
{
    NSSetUncaughtExceptionHandler(&uncaughtExceptionHandler);
    [FlurryAPI startSession:@"H9C9B8RUFY5KJSIXBKW1"];
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
    [_window release];
	[_glView release];
    [super dealloc];
}


+ (APPNAMEAppDelegate*) getInstance
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


#if defined (ENABLE_OPENFEINT)
- (void)applicationDidBecomeActive:(UIApplication *)application
{
    if(openFeintInitialized == YES) {
        [OpenFeint applicationDidBecomeActive];
    }
}


- (void)applicationWillResignActive:(UIApplication *)application
{
    if(openFeintInitialized == YES) {
        [OpenFeint applicationWillResignActive];
    }
}


- (void)applicationWillTerminate:(UIApplication*)application
{
    if(openFeintInitialized == YES) {
        [OpenFeint shutdown];
    }
}


- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    if(openFeintInitialized == YES) {
        [OpenFeint applicationDidRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
    }
}


- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    if(openFeintInitialized == YES) {
        [OpenFeint applicationDidFailToRegisterForRemoteNotifications];
    }
}


- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    if(openFeintInitialized == YES) {
        [OpenFeint applicationDidReceiveRemoteNotification:userInfo];
    }
}
#endif

@end