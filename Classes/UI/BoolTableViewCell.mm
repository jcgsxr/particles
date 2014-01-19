//
//  BoolTableViewCell.m
//  particles
//
//  Created by Johnny Chan on 6/9/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import "BoolTableViewCell.h"
#import "Engine.h"
#import "MainScreen.h"
#import "ParticleSystem.h"


@implementation BoolTableViewCell

@synthesize label, description, boolSwitch;

- (void)init:(int)theIndex
{	
	index = theIndex;
	[boolSwitch addTarget:self action:@selector(toggle) forControlEvents:UIControlEventValueChanged];
}

-(void)toggle
{
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	// Store the current switch value and apply it to the property mode.
	propVal val;
	val.boolVal = boolSwitch.on;
	
	// Set the bool value for all masses.
	for(int i = 0; i < mainscr->getParticleSys()->getNumMasses(); ++i)
	{
		mainscr->getParticleSys()->setPropertyValue(i, index, val);
	}
}

- (void)dealloc
{
	[label release];
	[description release];
	[boolSwitch release];
    [super dealloc];
}


@end
