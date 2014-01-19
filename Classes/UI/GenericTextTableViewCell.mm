//
//  GenericTextTableViewCell.mm
//  particles
//
//  Created by Johnny Chan on 6/11/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import "GenericTextTableViewCell.h"


@implementation GenericTextTableViewCell

@synthesize label, description;

- (void)init:(int)theIndex
{	
	index = theIndex;
}



- (void)dealloc 
{
	[label release];
	[description release];
    [super dealloc];
}


@end
