//
//  ImageSelectorViewController.h
//  particles
//
//  Created by Johnny Chan on 6/9/10.
//  Copyright 2010 LlamaFace. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface ImageSelectorViewController : UIViewController 
<UITableViewDataSource, UITableViewDelegate> 
{
	IBOutlet UITableView *table;
	
	IBOutlet UIBarButtonItem *bbItem;
	
	IBOutlet UINavigationBar *navBar;
}
@property (nonatomic, retain) IBOutlet UITableView *table;
@property (nonatomic, retain) IBOutlet UIBarButtonItem *bbItem;
@property (nonatomic, retain) IBOutlet UINavigationBar *navBar;

@end
