//
// COPIngredientListViewController.h
// CopelandiOS
//
// Created by Brock Boland on 6/14/13.
// Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Recipe.h"
#import "Ingredient.h"

@interface COPIngredientListViewController : UITableViewController

@property (nonatomic, strong) Recipe *currentRecipe;

@end
