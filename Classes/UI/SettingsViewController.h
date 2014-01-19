//
//  SettingsViewController.h
//  particles
//
//  Created by Johnny Chan on 6/6/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SettingsViewController : UIViewController 
<UITableViewDataSource, UITableViewDelegate> 
{
	IBOutlet UITableView *table;
	IBOutlet UINavigationBar *navBar;
	IBOutlet UIBarButtonItem *bbItem;

	NSMutableArray *settingsTableData;
}
@property (nonatomic, retain) IBOutlet UITableView *table;
@property (nonatomic, retain) IBOutlet UINavigationBar *navBar;
@property (nonatomic, retain) IBOutlet UIBarButtonItem *bbItem;

@property (nonatomic, retain) NSMutableArray *settingsTableData;

- (void)reload;
- (void)dismiss;

@end
