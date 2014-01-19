//
//  NumberTableViewCell.h
//  particles
//
//  Created by Johnny Chan on 6/7/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface NumberTableViewCell : UITableViewCell <UITextFieldDelegate>
{
	// The short description of the particle property.
	IBOutlet UILabel *label;
	// The long description of the particle property.
	IBOutlet UILabel *description;
	
	// Holds the property value of the particle.
	IBOutlet UITextField *textField;
	
	IBOutlet UIButton *plusButton;
	IBOutlet UIButton *minusButton;
	
	// Stores the particle property id.
	int index;
	
	// The number table cell can either hold an int or float value. 
	// Only one is used per cell and depends on what data type the particle property uses.
	int intVal;
	float floatVal;
}
@property (nonatomic, retain) IBOutlet UILabel *label;
@property (nonatomic, retain) IBOutlet UILabel *description;
@property (nonatomic, retain) IBOutlet UITextField *textField;
@property (nonatomic, retain) IBOutlet UIButton *plusButton;
@property (nonatomic, retain) IBOutlet UIButton *minusButton;


- (void)init:(int)theIndex;
- (void)setIntValue:(int)theInt;
- (void)setFloatValue:(float)theFloat;

@end
