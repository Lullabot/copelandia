//
// COPRecipeDetailViewController.m
// CopelandiOS
//
// Created by Brock Boland on 6/14/13.
// Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "COPRecipeDetailViewController.h"
#import "COPIngredientListViewController.h"

@interface COPRecipeDetailViewController ()

@end

@implementation COPRecipeDetailViewController

- (id)initWithStyle:(UITableViewStyle)style {
  self = [super initWithStyle:style];
  if (self) {
    // Custom initialization
  }
  return self;
}

- (void)viewDidLoad {
  [super viewDidLoad];
}

- (void) viewWillAppear:(BOOL)animated {
  [super viewWillAppear:animated];

  [self populateTableData];
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}




- (void)populateTableData {
  UITableView *table = [self tableView];

  NSInteger index = 0;
  NSIndexPath *path = nil;
  UITableViewCell *cell = nil;
  id temp = nil;

  while (index <= 8) {
    path = [NSIndexPath indexPathForRow:index inSection:0];
    cell = [table cellForRowAtIndexPath:path];
    switch (index) {
      case 0: // Title
        [[cell detailTextLabel] setText:self.currentRecipe.title];
        break;
      case 1: // Source
        [[cell detailTextLabel] setText:self.currentRecipe.source];
        break;
      case 2: // Submitted By
        [[cell detailTextLabel] setText:self.currentRecipe.submittedBy];
        break;
      case 3: // Prep time
        [[cell detailTextLabel] setText:self.currentRecipe.prepTime];
        break;
      case 4: // Cook time
        [[cell detailTextLabel] setText:self.currentRecipe.cookTime];
        break;
      case 5: // Servings
        [[cell detailTextLabel] setText:self.currentRecipe.cookTime];
        break;
      case 6: // Ingredients
        temp = [[self currentRecipe] valueForKey:@"ingredients"];
        [[cell detailTextLabel] setText:[NSString stringWithFormat:@"%i", [temp count]]];
        break;
      case 7: // Body
        temp = [cell viewWithTag:1201];
        [temp setText:self.currentRecipe.body];
        [temp sizeToFit];
        [cell sizeToFit];
        break;
      case 8: // Instructions
        temp = [cell viewWithTag:1202];
        [temp setText:self.currentRecipe.instructions];
        [temp sizeToFit];
        [cell sizeToFit];
        break;
    }
    ++index;
  }
}

#pragma mark - Prep for segue
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
  if ([[segue identifier] isEqualToString:@"View Ingredients"]) {
    COPIngredientListViewController *cont = (COPIngredientListViewController *)[segue destinationViewController];
    cont.currentRecipe = self.currentRecipe;
    cont.title = @"Ingredients";
  }
}
@end
