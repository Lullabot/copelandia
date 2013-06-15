//
//  COPRecipeDetailViewController.h
//  CopelandiOS
//
//  Created by Brock Boland on 6/14/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Recipe.h"

@interface COPRecipeDetailViewController : UITableViewController

- (void)populateTableData;

@property (nonatomic, strong) Recipe *currentRecipe;

@end
