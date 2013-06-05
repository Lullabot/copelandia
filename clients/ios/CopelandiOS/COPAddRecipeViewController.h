//
//  COPAddRecipeViewController.h
//  CopelandiOS
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Recipe.h"


@protocol COPAddRecipeViewControllerDelegate;


@interface COPAddRecipeViewController : UIViewController
@property (strong, nonatomic) IBOutlet UITextField *titleField;
@property (strong, nonatomic) IBOutlet UITextField *submittedByField;
@property (strong, nonatomic) IBOutlet UITextField *bodyField;

@property (weak, nonatomic) id <COPAddRecipeViewControllerDelegate> delegate;

@property (nonatomic, strong) Recipe *currentRecipe;




- (IBAction)cancel:(id)sender;
- (IBAction)save:(id)sender;

@end


@protocol COPAddRecipeViewControllerDelegate

-(void) addRecipeViewControllerDidSave;
-(void) addRecipeViewControllerDidCancel:(Recipe *)recipeToDelete;

@end