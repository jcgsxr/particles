//
//  SettingsController.mm
//  particles
//
//  Created by Johnny Chan on 6/6/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import "SettingsController.h"
#import "MainScreen.h"
#import "Engine.h"
#import "particlesAppDelegate.h"


@implementation SettingsController

@synthesize navController;

- (void)viewDidLoad
{
	
}


// Use this to show the modal view (pops-up from the bottom)`
- (void) showModal:(UIView*) modalView
{
	UIWindow* mainWindow = [particlesAppDelegate getInstance].window;

	CGSize offSize = [UIScreen mainScreen].bounds.size;
	
	CGPoint middleCenter = CGPointMake(offSize.width / 2.0, offSize.height / 2.0);

	CGPoint offScreenCenter = CGPointMake(offSize.width / 2.0, offSize.height * 1.5);
	modalView.center = offScreenCenter; // we start off-screen
	[mainWindow addSubview:modalView];
	
	// Show it with a transition effect
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration: (float)GL_TO_MODAL_TIME / 1000]; // animation duration in seconds
	
	// Make it go back to the original center.
	modalView.center = middleCenter;
	
	// Start the animation.
	[UIView commitAnimations];
}


// Use this to slide the semi-modal view back down.
- (void) hideModal:(UIView*) modalView
{
	CGSize offSize = [UIScreen mainScreen].bounds.size;
	CGPoint offScreenCenter = CGPointMake(offSize.width / 2.0, offSize.height * 1.5);
	[UIView beginAnimations:nil context:modalView];
	[UIView setAnimationDuration: (float)MODAL_TO_GL_TIME / 1000];
	[UIView setAnimationDelegate:self];
	
	[UIView setAnimationDidStopSelector:@selector(hideModalEnded:finished:context:)];
	
	modalView.center = offScreenCenter;
	[UIView commitAnimations];
}

- (void) hideModalEnded:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	UIView* modalView = (UIView *)context;
	[modalView removeFromSuperview];
	//[modalView release];
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return interfaceOrientation == UIInterfaceOrientationPortrait;	
}


- (void)dealloc
{
	[navController release];
	[super dealloc];
}


@end
