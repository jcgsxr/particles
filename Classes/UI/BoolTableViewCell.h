//
//  BoolTableViewCell.h
//  particles
//
//  Created by Johnny Chan on 6/9/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface BoolTableViewCell : UITableViewCell 
{
	// The short description of the particle property.
	IBOutlet UILabel *label;
	// The long description of the particle property.
	IBOutlet UILabel *description;
	
	IBOutlet UISwitch *boolSwitch;

	// Stores the particle property id.
	int index;
}
@property (nonatomic, retain) IBOutlet UILabel *label;
@property (nonatomic, retain) IBOutlet UILabel *description;
@property (nonatomic, retain) IBOutlet UISwitch *boolSwitch;

- (void)init:(int)theIndex;

@end
