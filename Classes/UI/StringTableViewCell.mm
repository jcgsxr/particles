//
//  StringTableViewCell.m
//  particles
//
//  Created by Johnny Chan on 6/11/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import "StringTableViewCell.h"
#import "MainScreen.h"
#import "ParticleSystem.h"
#import "Engine.h"

@implementation StringTableViewCell

@synthesize label, description, billboardButton, pointButton, quadButton;

- (void)init:(int)theIndex
{	
	// Set actions for the buttons.
	[billboardButton addTarget:self action:@selector(setBillboardMode) forControlEvents:UIControlEventTouchDown];
	[pointButton addTarget:self action:@selector(setPointSpriteMode) forControlEvents:UIControlEventTouchDown];
	[quadButton addTarget:self action:@selector(setQuadMode) forControlEvents:UIControlEventTouchDown];
	
	// The index holds the property index.
	index = theIndex;
}

- (void)setBillboardMode
{
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	propVal val;
	val.intVal = (int)eParticleDrawModeBillBoard;
	
	// Set the draw mode for all masses.
	for(int i = 0; i < mainscr->getParticleSys()->getNumMasses(); ++i)
	{
		mainscr->getParticleSys()->setPropertyValue(i, index, val);
	}
}


- (void)setPointSpriteMode
{
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	propVal val;
	val.intVal = (int)eParticleDrawModePoint;
	
	// Set the draw mode for all masses.
	for(int i = 0; i < mainscr->getParticleSys()->getNumMasses(); ++i)
	{
		mainscr->getParticleSys()->setPropertyValue(i, index, val);
	}
}


- (void)setQuadMode
{
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	propVal val;
	val.intVal = (int)eParticleDrawModeNormal;
	
	// Set the draw mode for all masses.
	for(int i = 0; i < mainscr->getParticleSys()->getNumMasses(); ++i)
	{
		mainscr->getParticleSys()->setPropertyValue(i, index, val);
	}
}


- (void)dealloc
{
	[label release];
	[description release];
	[billboardButton release];
	[pointButton release];
	[quadButton release];
    [super dealloc];
}


@end
