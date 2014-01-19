//
//  StringTableViewCell.h
//  particles
//
//  Created by Johnny Chan on 6/11/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import <UIKit/UIKit.h>


// This was originally going to be a selection of strings describing a particle property.
// It became exclusively used for changing the rendering mode using 3 separate buttons.
// It now has nothing to do with strings, but I didn't rename it out of laziness.
@interface StringTableViewCell : UITableViewCell 
{
	// The short description of the particle property.
	IBOutlet UILabel *label;
	// The long description of the particle property.
	IBOutlet UILabel *description;
	
	// The 3 buttons for changing rendering mode.
	IBOutlet UIButton *billboardButton;	// Button for changing rendering mode to billboarding.
	IBOutlet UIButton *pointButton;		// Button for changing rendering mode to point sprites.
	IBOutlet UIButton *quadButton;		// Button for changing rendering mode to 2D quads.
	
	// Stores the particle property id.
	int index;
}

@property (nonatomic, retain) IBOutlet UILabel *label;
@property (nonatomic, retain) IBOutlet UILabel *description;
@property (nonatomic, retain) IBOutlet UIButton *billboardButton;
@property (nonatomic, retain) IBOutlet UIButton *pointButton;
@property (nonatomic, retain) IBOutlet UIButton *quadButton;

- (void)init:(int)theIndex;

@end
