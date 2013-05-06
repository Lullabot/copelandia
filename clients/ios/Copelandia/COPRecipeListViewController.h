//
//  COPRecipeListViewController.h
//  Copelandia
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "COPAddRecipeViewController.h"

@interface COPRecipeListViewController : UITableViewController <COPAddRecipeViewControllerDelegate, NSFetchedResultsControllerDelegate>

@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

@property (strong, nonatomic) NSFetchedResultsController *fetchedResultsController;

@end
