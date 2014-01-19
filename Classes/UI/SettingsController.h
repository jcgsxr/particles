//
//  SettingsController.h
//  particles
//
//  Created by Johnny Chan on 6/6/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SettingsController : UIViewController
{
	IBOutlet UINavigationController *navController;
}

@property (nonatomic, retain) UINavigationController *navController;

- (void) showModal:(UIView*) modalView;
- (void) hideModal:(UIView*) modalView;

@end
