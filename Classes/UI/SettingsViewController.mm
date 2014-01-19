//
//  SettingsViewController.mm
//  particles
//
//  Created by Johnny Chan on 6/6/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import "SettingsViewController.h"
#import "ParticleSystem.h"
#import "MainScreen.h"
#import "Engine.h"
#import "NumberTableViewCell.h"
#import "BoolTableViewCell.h"
#import "StringTableViewCell.h"
#import "GenericTextTableViewCell.h"
#import "ImageSelectorViewController.h"
#import "particlesAppDelegate.h"
#import "SettingsController.h"


@implementation SettingsViewController

@synthesize table, settingsTableData, navBar, bbItem;

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
	// Set the action for the barbuttonitem to dismiss the modal view.
	[bbItem setTarget:self];
	bbItem.action = @selector(dismiss);
	//bbItem = [[UIBarButtonItem alloc]
	//		  initWithBarButtonSystemItem:UIBarButtonSystemItemStop target:self action:@selector(dismiss)];

	// Load the table data into the data array.
	if (settingsTableData == nil)
	{
		settingsTableData = [[NSMutableArray alloc] initWithObjects: nil];
		for (int i = 0; i < PROP_MAX; ++i)
		{
			NSString *tmpstr = [NSString stringWithUTF8String:CParticleSystem::getPropName(i)];
			[settingsTableData insertObject:tmpstr atIndex:i];
		}
	}
	
    [super viewDidLoad];
}


- (void)dismiss
{
	SettingsController *sCtrl = [[particlesAppDelegate getInstance] settingsCtrl];
	[[[sCtrl.navController viewControllers] objectAtIndex:0] reload];
	SettingsViewController *svCtrl = [[sCtrl.navController viewControllers] objectAtIndex:0];
	[sCtrl hideModal: svCtrl.view];
	
    //[self dismissModalViewControllerAnimated:YES];
	
	// Resume the particle system.
	CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
	mainscr->dismissModalView();
}


-(void)reload 
{
    [table reloadData];
}


- (UITableViewCell *)tableView:(UITableView *)aTableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
//	static NSString *cellId = @"Cell";
	static NSString *numCellId = @"NumCell";
	static NSString *boolCellId = @"BoolCell";
	static NSString *strCellId = @"StringCell";
	static NSString *genericTextCellId = @"GenericTextCell";
	
	// Check what kind of table cell this should be.
	if ((CParticleSystem::getPropDataType(indexPath.row) == PROP_DATA_TYPE_INT) ||
		(CParticleSystem::getPropDataType(indexPath.row) == PROP_DATA_TYPE_FLOAT))
	{
		
		NumberTableViewCell *cell = (NumberTableViewCell*)[aTableView dequeueReusableCellWithIdentifier:numCellId];
		if (cell == nil)
		{
			NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"NumberTableViewCell" owner:self options:nil];
			cell = [nib objectAtIndex:0];
		}
		
		// Init the cell with the id of the cell. This is used later to send the cell data back to the particle system when setting properties.
		[cell init:indexPath.row];
		
		// Fill in the text field with the current mode's property value.
		CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
		cell.textField.text = [NSString stringWithUTF8String:mainscr->getPropertyValueStr(0, indexPath.row)];
		
		// Set the label to be the particle property name.
		cell.label.text = [NSString stringWithUTF8String:CParticleSystem::getPropName(indexPath.row)];
		
		// Set the 2nd label to be the property description.
		cell.description.text = [NSString stringWithUTF8String:CParticleSystem::getPropDesc(indexPath.row)];
		
		return cell;
	}
	else if (CParticleSystem::getPropDataType(indexPath.row) == PROP_DATA_TYPE_BOOL)
	{
		BoolTableViewCell *cell = (BoolTableViewCell*)[aTableView dequeueReusableCellWithIdentifier:boolCellId];
		if (cell == nil)
		{
			NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"BoolTableViewCell" owner:self options:nil];
			cell = [nib objectAtIndex:0];
		}
		
		[cell init:indexPath.row];
		
		// Set the initial switch value.
		CMainScreen *mainscr = (CMainScreen*)engine->getCurrentScreen();
		[cell.boolSwitch setOn:mainscr->getParticleSys()->getPropertyValue(0, indexPath.row).boolVal animated:NO];
		
		// Set the label to be the particle property name.
		cell.label.text = [NSString stringWithUTF8String:CParticleSystem::getPropName(indexPath.row)];
		
		// Set the 2nd label to be the property description.
		cell.description.text = [NSString stringWithUTF8String:CParticleSystem::getPropDesc(indexPath.row)];
		
		return cell;
	}
	else if (CParticleSystem::getPropDataType(indexPath.row) == PROP_DATA_TYPE_STR)
	{
		StringTableViewCell *cell = (StringTableViewCell*)[aTableView dequeueReusableCellWithIdentifier:strCellId];
		if (cell == nil)
		{
			NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"StringTableViewCell" owner:self options:nil];
			cell = [nib objectAtIndex:0];
		}
		
		[cell init:indexPath.row];
				
		// Set the label to be the particle property name.
		cell.label.text = [NSString stringWithUTF8String:CParticleSystem::getPropName(indexPath.row)];
		
		// Set the 2nd label to be the property description.
		cell.description.text = [NSString stringWithUTF8String:CParticleSystem::getPropDesc(indexPath.row)];
		
		return cell;
	}
	else
	{
		GenericTextTableViewCell *cell = (GenericTextTableViewCell*)[aTableView dequeueReusableCellWithIdentifier:genericTextCellId];
		
		if (cell == nil)
		{
			NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"GenericTextTableViewCell" owner:self options:nil];
			cell = [nib objectAtIndex:0];
		}
		
		[cell init:indexPath.row];
		
		// Set the label to be the particle property name.
		cell.label.text = [NSString stringWithUTF8String:CParticleSystem::getPropName(indexPath.row)];
		
		// Set the 2nd label to be the property description.
		cell.description.text = [NSString stringWithUTF8String:CParticleSystem::getPropDesc(indexPath.row)];
		
		return cell;
		
	}
}


- (NSInteger)tableView:(UITableView *)aTableView numberOfRowsInSection:(NSInteger)section
{
	return [settingsTableData count];
}


- (void)tableView:(UITableView *)aTableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
	if (indexPath.row != PROP_IMAGE_ID)
		return;
	
	ImageSelectorViewController *imageview = [[ImageSelectorViewController alloc] init];
	
	[aTableView deselectRowAtIndexPath:indexPath animated:YES];
	
	UIWindow* mainWindow = [particlesAppDelegate getInstance].window;
	[mainWindow addSubview: imageview.view];
	
	//[[[[[UIApplication sharedApplication] delegate] settingsCtrl] navController] pushViewController:imageview animated:YES];
	
	//[imageview release];
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
	[settingsTableData release];
    [super dealloc];
}


@end
