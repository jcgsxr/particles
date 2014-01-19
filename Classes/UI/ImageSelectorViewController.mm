//
//  ImageSelectorViewController.m
//  particles
//
//  Created by Johnny Chan on 6/9/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import "ImageSelectorViewController.h"
#import "MainScreen.h"
#import "Engine.h"
#import "ParticleSystem.h"
#import "types.h"


@implementation ImageSelectorViewController

@synthesize table, navBar, bbItem;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
	bbItem.target = self;
	bbItem.action = @selector(dismiss);
	
    [super viewDidLoad];
}


- (void) dismiss
{
	[self.view removeFromSuperview];
	[self release];
}


- (UITableViewCell *)tableView:(UITableView *)aTableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	static NSString *cellId = @"Cell";
	
	UITableViewCell *cell = [aTableView dequeueReusableCellWithIdentifier:cellId];
	
	if (cell == nil)
	{
		cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellId];
	}
	
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();

	int imageID = mainscr->getImageId(indexPath.row);
	
	// Set particle image and label.
	cell.image = [UIImage imageNamed:[NSString stringWithUTF8String:mainscr->getImageFileName(imageID)]];
	cell.textLabel.text = [NSString stringWithUTF8String:mainscr->getImageName(imageID)];
	cell.textLabel.textAlignment = UITextAlignmentCenter;
	cell.textLabel.textColor = RGB(255, 255, 255);
	
	return cell;
}


- (NSInteger)tableView:(UITableView *)aTableView numberOfRowsInSection:(NSInteger)section
{
	return imageMAX;
}


- (void)tableView:(UITableView*)aTableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	
	propVal val;

	// Set the image id.
	val.intVal = mainscr->getImageId(indexPath.row);
	mainscr->getParticleSys()->setPropertyValue(0, PROP_IMAGE_ID, val);
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc 
{
	[table release];
	[navBar release];
	[bbItem release];
    [super dealloc];
}


@end
