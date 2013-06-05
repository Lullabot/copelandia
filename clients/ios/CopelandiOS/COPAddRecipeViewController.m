//
//  COPAddRecipeViewController.m
//  CopelandiOS
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "COPAddRecipeViewController.h"

@interface COPAddRecipeViewController ()

@end

@implementation COPAddRecipeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    self.titleField.text = [self.currentRecipe title];
    self.submittedByField.text = [self.currentRecipe submittedBy];
    self.bodyField.text = [self.currentRecipe body];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)cancel:(id)sender {
    // Dismiss the modal and remove the object
    [self.delegate addRecipeViewControllerDidCancel:self.currentRecipe];
}

- (IBAction)save:(id)sender {
    // Dismiss the modal and save the object
    [self.currentRecipe setTitle:self.titleField.text];
    [self.currentRecipe setSubmittedBy:self.submittedByField.text];
    [self.currentRecipe setBody:self.bodyField.text];

    [self.delegate addRecipeViewControllerDidSave];
}
@end
