//
//  NumberTableViewCell.m
//  particles
//
//  Created by Johnny Chan on 6/7/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import "NumberTableViewCell.h"
#import "ParticleSystem.h"
#import "Engine.h"
#import "MainScreen.h"
#import "ParticleSystem.h"

@implementation NumberTableViewCell

@synthesize label, description, textField, plusButton, minusButton;

- (void)init:(int)theIndex
{
	// Set the textfield delegate to this class to get textfield callback alerts.
	textField.delegate = self;
	
	// Set actions for the buttons.
	[plusButton addTarget:self action:@selector(increment) forControlEvents:UIControlEventTouchDown];
	[minusButton addTarget:self action:@selector(decrement) forControlEvents:UIControlEventTouchDown];
	
	// The index holds the property index.
	index = theIndex;
}


- (void)increment
{
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	// Increment the property value for all mases.
	for(int i = 0; i < mainscr->getParticleSys()->getNumMasses(); ++i)
	{
		mainscr->setPropertyInc(i, index, PROP_DIR_INC);
	}

	// Set the textfield text.
	textField.text = [NSString stringWithUTF8String:mainscr->getPropertyValueStr(0, index)];
}


-(void)decrement
{
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	// Decrement the property value for all mases.
	for(int i = 0; i < mainscr->getParticleSys()->getNumMasses(); ++i)
	{
		mainscr->setPropertyInc(i, index, PROP_DIR_DEC);
	}	
	
	// Set the textfield text.
	textField.text = [NSString stringWithUTF8String:mainscr->getPropertyValueStr(0, index)];
}


- (void)setIntValue:(int)theInt
{
	intVal = theInt;
}


- (void)setFloatValue:(float)theFloat
{
	floatVal = theFloat;
}



- (void)textFieldDidEndEditing:(UITextField*)txtField
{	
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	// Set the text.
	textField.text = txtField.text;
	
	propVal val;
	
	// Determine whether this cell holds an or a float.
	if (CParticleSystem::getPropDataType(index) == PROP_DATA_TYPE_INT)
	{
		// Get the integer value of the field. 
		// TODO: Check for a valid entry, that is, an integer.
		intVal = [txtField.text intValue];
		val.intVal = intVal;
	}
	else if (CParticleSystem::getPropDataType(index) == PROP_DATA_TYPE_FLOAT)
	{
		// Get the float value of the field. 
		// TODO: Check for a valid entry, that is, a float.
		floatVal = [txtField.text floatValue];
		val.floatVal = floatVal;
	}

	
	// Set the property value for all masses.
	for(int i = 0; i < mainscr->getParticleSys()->getNumMasses(); ++i)
	{
		mainscr->getParticleSys()->setPropertyValue(i, index, val);
	}
	
	if (CParticleSystem::getPropDataType(index) == PROP_DATA_TYPE_INT)
	{
		// TODO: Separate out the individual masses.
		propVal tmp  = mainscr->getParticleSys()->getPropertyValue(0, index);
		
		// Now reformat the text after the int/float value was determined.
		textField.text = [NSString stringWithFormat:@"%d", tmp.intVal];
	}
	else
	{
		// TODO: Separate out the individual masses.
		propVal tmp = mainscr->getParticleSys()->getPropertyValue(0, index);
		
		// Now reformat the text after the int/float value was determined.
		textField.text = [NSString stringWithFormat:@"%4.2f", tmp.floatVal];
	}
}


- (void) textFieldDidBeginEditing:(UITextField*)txtField
{
	// Clear out any existing value with zero.
	// TODO: figure out a better way to place the keyboard so that it doesn't block the textfield.
	textField.text = @"0";
}


- (BOOL)textFieldShouldReturn:(UITextField *)txtField
{
	// the user pressed the "Done" button, so dismiss the keyboard
	[txtField resignFirstResponder];
	return YES;
}


- (void)dealloc 
{
	[label release];
	[description release];
	[textField release];
	[plusButton release];
	[minusButton release];
    [super dealloc];
}


@end
