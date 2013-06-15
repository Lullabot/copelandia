//
//  COPRecipeListViewController.m
//  CopelandiOS
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "COPRecipeListViewController.h"
#import "COPRecipeDetailViewController.h"
#import "Recipe.h"
#import "Ingredient.h"
#import "AFJSONRequestOperation.h"

#define kNodeListJson @"http://copelandia.lulladev.com/node.json"

@interface COPRecipeListViewController ()

@end

@implementation COPRecipeListViewController

@synthesize fetchedResultsController = _fetchedResultsController;

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

    NSError *error = nil;
    if (![[self fetchedResultsController] performFetch:&error]) {
        NSLog(@"Fetch error: %@", error);
        abort();
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [[self.fetchedResultsController sections] count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    id <NSFetchedResultsSectionInfo> secInfo = [[self.fetchedResultsController sections] objectAtIndex:section];
    return [secInfo numberOfObjects];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];

    Recipe *recipe = [self.fetchedResultsController objectAtIndexPath:indexPath];
    cell.textLabel.text = recipe.title;

    return cell;
}



#pragma mark - Fetched Results Controller

- (NSFetchedResultsController *)fetchedResultsController {
    if (_fetchedResultsController != nil) {
        return _fetchedResultsController;
    }
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Recipe"
                                              inManagedObjectContext:self.managedObjectContext];
    [fetchRequest setEntity:entity];

    NSSortDescriptor *sortDescriptor = [[NSSortDescriptor alloc] initWithKey:@"title" ascending:YES];
    NSArray *sortDescriptors = [[NSArray alloc] initWithObjects:sortDescriptor, nil];
    [fetchRequest setSortDescriptors:sortDescriptors];
    
    _fetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:fetchRequest
                                                                    managedObjectContext:self.managedObjectContext
                                                                      sectionNameKeyPath:nil
                                                                               cacheName:nil];
    _fetchedResultsController.delegate = self;
    return _fetchedResultsController;
}

-(void)controllerWillChangeContent:(NSFetchedResultsController *)controller {
    [self.tableView beginUpdates];
}

-(void)controllerDidChangeContent:(NSFetchedResultsController *)controller {
    [self.tableView endUpdates];
}

-(void)controller:(NSFetchedResultsController *)controller didChangeObject:(id)anObject atIndexPath:(NSIndexPath *)indexPath forChangeType:(NSFetchedResultsChangeType)type newIndexPath:(NSIndexPath *)newIndexPath {
    
    UITableView *tableView = self.tableView;
    switch(type) {
        case NSFetchedResultsChangeInsert:
            [tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationFade];
            break;
            
        case NSFetchedResultsChangeDelete:
            [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
            break;
            
        case NSFetchedResultsChangeUpdate: {
            Recipe *r = [self.fetchedResultsController objectAtIndexPath:indexPath];
            UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
            cell.textLabel.text = r.title;
        }
            break;
            
        case NSFetchedResultsChangeMove:
            [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
            [tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:newIndexPath] withRowAnimation:UITableViewRowAnimationFade];
            break;
            
    }
}

-(void)controller:(NSFetchedResultsController *)controller didChangeSection:(id<NSFetchedResultsSectionInfo>)sectionInfo atIndex:(NSUInteger)sectionIndex forChangeType:(NSFetchedResultsChangeType)type {
    switch (type) {
        case NSFetchedResultsChangeInsert:
            [self.tableView insertSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
            break;
            
        case NSFetchedResultsChangeDelete:
            [self.tableView deleteSections:[NSIndexSet indexSetWithIndex:sectionIndex] withRowAnimation:UITableViewRowAnimationFade];
            break;
            
    }
}




#pragma mark - Prep for segue
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([[segue identifier] isEqualToString:@"ViewRecipe"]) {
        COPRecipeDetailViewController *cont = (COPRecipeDetailViewController *)[segue destinationViewController];
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        cont.currentRecipe = [self.fetchedResultsController objectAtIndexPath:indexPath];
        cont.title = cont.currentRecipe.title;
    }
}


// Refresh the list of recipes from Drupal
- (IBAction)refreshList:(id)sender {
    NSURL *url = [NSURL URLWithString:kNodeListJson];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];

    AFJSONRequestOperation *operation = [AFJSONRequestOperation JSONRequestOperationWithRequest:request success:^(NSURLRequest *request, NSHTTPURLResponse *response, id JSON) {
        for (id node in [JSON valueForKeyPath:@"list"]) {
            // Check to see if this is a new or locally-existing node
            NSString *nid = [node valueForKey:@"nid"];

            NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
            NSEntityDescription *entity = [NSEntityDescription entityForName:@"Recipe" inManagedObjectContext:[self managedObjectContext]];
            [fetchRequest setEntity:entity];

            NSPredicate *predicate = [NSPredicate predicateWithFormat:@"nid == %@", nid];
            [fetchRequest setPredicate:predicate];

            Recipe *newRecipe;

            NSError *error = nil;
            NSArray *fetchedObjects = [[self managedObjectContext] executeFetchRequest:fetchRequest error:&error];
            if (fetchedObjects != nil) {
                // No items found
                if ([fetchedObjects count] == 0) {
                    // Create a new recipe
                    newRecipe = (Recipe *)[NSEntityDescription insertNewObjectForEntityForName:@"Recipe" inManagedObjectContext:self.managedObjectContext];
                    newRecipe.nid = [node valueForKey:@"nid"];
                }
                else {
                    newRecipe = [fetchedObjects objectAtIndex:0];
                }
            }
            else {
                // Fetch request failed or whatever
                // Create a new recipe
                newRecipe = (Recipe *)[NSEntityDescription insertNewObjectForEntityForName:@"Recipe" inManagedObjectContext:self.managedObjectContext];
                newRecipe.nid = [node valueForKey:@"nid"];
            }


            if ([[node valueForKey:@"title"] isKindOfClass:[NSString class]]) {
                newRecipe.title = [node valueForKey:@"title"];
            }

            if ([[[node valueForKey:@"body"] valueForKey:@"value"] isKindOfClass:[NSString class]]) {
                newRecipe.body = [[node valueForKey:@"body"] valueForKey:@"value"];
            }

            if ([[[node valueForKey:@"field_recipe_instructions"] valueForKey:@"value"] isKindOfClass:[NSString class]]) {
                newRecipe.instructions = [[node valueForKey:@"field_recipe_instructions"] valueForKey:@"value"];
            }

            if ([[node valueForKey:@"field_recipe_source"] isKindOfClass:[NSString class]]) {
                newRecipe.source = [node valueForKey:@"field_recipe_source"];
            }

            // Ingredients
            for (id ingredient in [node valueForKey:@"field_recipe_ingredients"]) {
                NSString *ingredientUrlString = [[ingredient valueForKey:@"uri"] stringByAppendingPathExtension:@"json"];
                NSURL *ingredientUrl = [NSURL URLWithString:ingredientUrlString];
                NSData *responseData = [NSData dataWithContentsOfURL:ingredientUrl];
                if (responseData) {
                    NSError *ingredientError = nil;

                    NSDictionary *ingredientJSON = [NSJSONSerialization JSONObjectWithData:responseData
                                                                                   options:kNilOptions
                                                                                     error:&ingredientError];
                    if (ingredientError) {
                        NSLog(@"Error: %@", ingredientError);
                    }
                    else {
                        NSString *ingredientItemID = [ingredientJSON objectForKey:@"item_id"];

                        // Create a fetch request to look for an existing ingredient
                        NSFetchRequest *ingredientFetchRequest = [[NSFetchRequest alloc] init];
                        NSEntityDescription *ingredientEntityDesc = [NSEntityDescription entityForName:@"Ingredient" inManagedObjectContext:[self managedObjectContext]];
                        [ingredientFetchRequest setEntity:ingredientEntityDesc];

                        NSPredicate *ingredientPredicate = [NSPredicate predicateWithFormat:@"itemID == %@", ingredientItemID];
                        [ingredientFetchRequest setPredicate:ingredientPredicate];

                        Ingredient *ingredientMO;

                        NSError *ingredientFetchError = nil;
                        NSArray *fetchedIngredients = [[self managedObjectContext] executeFetchRequest:ingredientFetchRequest error:&ingredientFetchError];
                        if (fetchedIngredients != nil && [fetchedIngredients count] > 0) {
                            ingredientMO = [fetchedIngredients objectAtIndex:0];
                        }
                        else {
                            // Ingredient not found, create a new one
                            ingredientMO = (Ingredient *)[NSEntityDescription insertNewObjectForEntityForName:@"Ingredient" inManagedObjectContext:self.managedObjectContext];
                            ingredientMO.itemID = ingredientItemID;
                        }

                        // Set the amount and ingredient type on the Ingredient object
                        ingredientMO.amount = [ingredientJSON objectForKey:@"field_ingredient_amount"];
                        ingredientMO.ingredientName = [ingredientJSON objectForKey:@"field_ingredient_ingredient"];

                        // Save this Ingredient to the Recipe
                        ingredientMO.recipe = newRecipe;
                    }
                }
            }

            error = nil;
            if (![self.managedObjectContext save:&error]) {
                NSLog(@"Error saving: %@", error);
            }
            [self dismissViewControllerAnimated:YES completion:nil];
        }
    } failure:^( NSURLRequest *request , NSHTTPURLResponse *response , NSError *error , id JSON ) {
        NSLog(@"Failed to request node list.");
    }];

    [operation start];
}


@end


