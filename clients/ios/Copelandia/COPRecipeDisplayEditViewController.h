//
//  COPRecipeDisplayEditViewController.h
//  Copelandia
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Recipe.h"

@interface COPRecipeDisplayEditViewController : UIViewController

@property (nonatomic, strong) Recipe *currentRecipe;

@property (strong, nonatomic) IBOutlet UITextField *titleField;
@property (strong, nonatomic) IBOutlet UITextField *submittedByField;
@property (strong, nonatomic) IBOutlet UITextField *bodyField;


- (IBAction)starteEditing:(id)sender;
- (IBAction)doneEditing:(id)sender;

@property (strong, nonatomic) IBOutlet UIButton *editButton;
@property (strong, nonatomic) IBOutlet UIButton *doneButton;

@end
