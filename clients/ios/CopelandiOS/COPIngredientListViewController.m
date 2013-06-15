//
//  COPIngredientListViewController.m
//  CopelandiOS
//
//  Created by Brock Boland on 6/14/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "COPIngredientListViewController.h"
#import "Ingredient.h"

@interface COPIngredientListViewController ()

@property NSArray *orderedIngredients;

@end

@implementation COPIngredientListViewController


- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.orderedIngredients = [self.currentRecipe.ingredients allObjects];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return [self.currentRecipe.ingredients count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Ingredient";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];
    
    // Configure the cell...
    Ingredient *ingredient = [self.orderedIngredients objectAtIndex:indexPath.row];
    [[cell detailTextLabel] setText:ingredient.amount];
    [[cell textLabel] setText:ingredient.ingredientName];

    return cell;
}


@end
