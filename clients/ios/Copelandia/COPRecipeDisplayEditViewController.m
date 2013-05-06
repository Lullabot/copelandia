//
//  COPRecipeDisplayEditViewController.m
//  Copelandia
//
//  Created by Brock Boland on 5/5/13.
//  Copyright (c) 2013 Lullabot. All rights reserved.
//

#import "COPRecipeDisplayEditViewController.h"
#import "COPAppDelegate.h"

@interface COPRecipeDisplayEditViewController ()

@end

@implementation COPRecipeDisplayEditViewController

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
    
    self.titleField.text = self.currentRecipe.title;
    self.bodyField.text = self.currentRecipe.body;
    self.submittedByField.text = self.currentRecipe.submittedBy;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)starteEditing:(id)sender {
    self.doneButton.hidden = NO;
    self.editButton.hidden = YES;
    
    self.titleField.enabled = YES;
    self.bodyField.enabled = YES;
    self.submittedByField.enabled = YES;

    self.titleField.borderStyle = UITextBorderStyleRoundedRect;
    self.bodyField.borderStyle = UITextBorderStyleRoundedRect;
    self.submittedByField.borderStyle = UITextBorderStyleRoundedRect;
}

- (IBAction)doneEditing:(id)sender {
    self.doneButton.hidden = YES;
    self.editButton.hidden = NO;

    self.currentRecipe.title = self.titleField.text;
    self.currentRecipe.submittedBy = self.submittedByField.text;
    self.currentRecipe.body = self.bodyField.text;
    
    self.titleField.borderStyle = UITextBorderStyleNone;
    self.bodyField.borderStyle = UITextBorderStyleNone;
    self.submittedByField.borderStyle = UITextBorderStyleNone;

    
    self.titleField.enabled = NO;
    self.bodyField.enabled = NO;
    self.submittedByField.enabled = NO;
    
    COPAppDelegate *myApp = (COPAppDelegate *)[[UIApplication sharedApplication] delegate];
    [myApp saveContext];
}
@end
