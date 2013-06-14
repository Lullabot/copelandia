//
//  COPRecipeListViewController.m
//  CopelandiOS
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "COPRecipeListViewController.h"
#import "COPRecipeDisplayEditViewController.h"
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

    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    
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
    
    // Configure the cell...
    Recipe *recipe = [self.fetchedResultsController objectAtIndexPath:indexPath];
    cell.textLabel.text = recipe.title;
    
    return cell;
}

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete an object
        Recipe *recipe = [self.fetchedResultsController objectAtIndexPath:indexPath];
        [self.managedObjectContext deleteObject:recipe];

        NSError *error = nil;
        if (![self.managedObjectContext save:&error]) {
            NSLog(@"Error deleting object: %@", error);
        }
    }
}


/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/



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



#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Navigation logic may go here. Create and push another view controller.
}



#pragma mark - Prep for segue
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([[segue identifier] isEqualToString:@"ViewRecipe"]) {
        COPRecipeDisplayEditViewController *cont = (COPRecipeDisplayEditViewController *)[segue destinationViewController];
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        cont.currentRecipe = [self.fetchedResultsController objectAtIndexPath:indexPath];
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
                // Nothing
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
                        // Create a new ingredient in the recipe
                        Ingredient *ingredientMO = (Ingredient *)[NSEntityDescription insertNewObjectForEntityForName:@"Ingredient" inManagedObjectContext:self.managedObjectContext];
                        ingredientMO.amount = [ingredientJSON objectForKey:@"field_ingredient_amount"];
                        ingredientMO.ingredient = [ingredientJSON objectForKey:@"field_ingredient_ingredient"];
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


