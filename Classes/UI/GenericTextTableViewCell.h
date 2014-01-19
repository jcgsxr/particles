//
//  GenericTextTableViewCell.h
//  particles
//
//  Created by Johnny Chan on 6/11/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import <UIKit/UIKit.h>


// This table cell is used mainly for properties that, when selected, transition to another table.
// There isn't anything besides text and the property id in this table cell.
@interface GenericTextTableViewCell : UITableViewCell 
{
	// The short description of the particle property.
	IBOutlet UILabel *label;
	// The long description of the particle property.
	IBOutlet UILabel *description;

	// Stores the particle property id.
	int index;
}
@property (nonatomic, retain) IBOutlet UILabel *label;
@property (nonatomic, retain) IBOutlet UILabel *description;

- (void)init:(int)theIndex;

@end
