#ifndef __APPDELEGATE_H__
#define __APPDELEGATE_H__

#import <UIKit/UIKit.h>
#import "Engine.h"
//#import "TouchSystem.h"

#define FRAME_INTERVAL	(1.0/60.0)


@class EAGLView;
@class particlesAppDelegate;
@class SettingsController;

@interface particlesAppDelegate : NSObject

// Particle settings nav controller
@property (nonatomic, retain) IBOutlet SettingsController *settingsCtrl;
@property (nonatomic, retain) UIWindow* window;
@property (nonatomic, retain) EAGLView* glView;

- (int) getCurrentTimeMillis;
+ (particlesAppDelegate*) getInstance;
- (void) setLandscapeMode;
- (void) setPortraitMode;
- (EAGLView*) getView;


@end

#endif