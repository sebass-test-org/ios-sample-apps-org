//
//  OptionsViewController.m
//  OptionsSampleApp
//
//  Created on 12/18/14.
//  Copyright (c) 2014 ooyala. All rights reserved.
//

#import "OptionsViewController.h"
#import <OoyalaSDK/OoyalaSDK.h>
#import "AppDelegate.h"

@interface OptionsViewController () <UITextFieldDelegate>

#pragma mark - Private properties

@property IBOutlet UIButton *button;
@property IBOutlet UIView *playerView;
@property (nonatomic) OOOoyalaPlayerViewController* playerViewController;
@property NSString *nib;
@property NSString *pcode;
@property NSString *playerDomain;
@property NSString *embedCode;

@end

@implementation OptionsViewController{
    AppDelegate *appDel;
}

#pragma mark - Synthesize

@synthesize switchLabel1 = _switchLabel1;
@synthesize switchLabel2 = _switchLabel2;
@synthesize switch1 = _switch1;
@synthesize switch2 = _switch2;
@synthesize text1 = _text1;
@synthesize text2 = _text2;
@dynamic playerView;

#pragma mark - Initialization

- (instancetype)initWithPlayerSelectionOption:(PlayerSelectionOption *)playerSelectionOption
                                qaModeEnabled:(BOOL)qaModeEnabled {
  self = [super initWithPlayerSelectionOption:playerSelectionOption qaModeEnabled:qaModeEnabled];
  if (playerSelectionOption.nib) {
    _nib = playerSelectionOption.nib;
  } else {
    _nib = @"PlayerDoubleSwitch";
  }
  if (self.playerSelectionOption) {
    _embedCode = self.playerSelectionOption.embedCode;
    self.title = self.playerSelectionOption.title;
    _pcode = self.playerSelectionOption.pcode;
    _playerDomain = self.playerSelectionOption.domain;
  } else {
    NSLog(@"There was no PlayerSelectionOption!");
    return nil;
  }
  return self;
}

#pragma mark - Life cycle

- (void)loadView {
  [super loadView];
  [[NSBundle mainBundle] loadNibNamed:self.nib owner:self options:nil];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  
  appDel = (AppDelegate *)UIApplication.sharedApplication.delegate;
  
  // In QA Mode , making textView visible
  self.textView.hidden = !self.qaModeEnabled;
  
  if (_switch1) {
    _switchLabel1.text = @"ShowPromoImage";
    _switch1.on = NO;
  }
  
  if (_switch2) {
    _switchLabel2.text = @"Preload";
    _switch2.on = YES;
  }
  
  if (_text1) {
    _switchLabel1.text = @"n/w timeout";
    _text1.text = @"60.0";
    _text1.delegate = self;
  }
  
  if (_text2) {
    _switchLabel2.text = @"";
    _switchLabel2.enabled = NO;
    _text2.enabled = NO;
  }
  
  [_button setTitle:@"Create" forState:UIControlStateNormal];
  
  self.view.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
}

#pragma mark - Actions

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

- (IBAction)onButtonClick:(id)sender {
  OOOptions *options = [OOOptions new];
  
  if (self.switch1) {
    options.showPromoImage = self.switch1.on;
  }
  
  if (self.switch2) {
    options.preloadContent = self.switch2.on;
  }
  
  if (self.text1) {
    NSTimeInterval timeout = self.text1.text.floatValue;
    options.connectionTimeout = timeout;
  }
  
  if (self.playerViewController) {
    [self.playerViewController removeFromParentViewController];
    [self.playerViewController.view removeFromSuperview];
  }
  
  // Create Ooyala ViewController
  OOOoyalaPlayer *player = [[OOOoyalaPlayer alloc] initWithPcode:self.pcode
                                                          domain:[[OOPlayerDomain alloc] initWithString:self.playerDomain] options:options];
  _playerViewController = [[OOOoyalaPlayerViewController alloc] initWithPlayer:player];
  
  [NSNotificationCenter.defaultCenter addObserver:self
                                         selector:@selector(notificationHandler:)
                                             name:nil
                                           object:self.playerViewController.player];
  
  // Setup video view
  CGRect rect = self.playerView.bounds;
  _playerViewController.view.frame = rect;
  [self addChildViewController:_playerViewController];
  [self.playerView addSubview:_playerViewController.view];
  
  [_playerViewController.player setEmbedCode:self.embedCode];
  
  if (_initialTime > 0) {
    [_playerViewController.player playWithInitialTime:_initialTime];
  }
}

#pragma mark - UITextFieldDelegate

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
  [textField resignFirstResponder];
  return YES;
}

#pragma mark - Private functions

- (void)notificationHandler:(NSNotification*)notification {
  // Ignore TimeChangedNotificiations for shorter logs
  if ([notification.name isEqualToString:OOOoyalaPlayerTimeChangedNotification]) {
    return;
  }
  
  NSString *message = [NSString stringWithFormat:@"Notification Received: %@. state: %@. playhead: %f count: %d",
                       notification.name,
                       [OOOoyalaPlayer playerStateToString:[self.playerViewController.player state]],
                       [self.playerViewController.player playheadTime], appDel.count];
  
  NSLog(@"%@",message);
  
  // In QA Mode , adding notifications to the TextView
  if (self.qaModeEnabled) {
    NSString *string = self.textView.text;
    NSString *appendString = [NSString stringWithFormat:@"%@ :::::::::: %@",string,message];
    dispatch_async(dispatch_get_main_queue(), ^{
      self.textView.text = appendString;
    });
  }
  
  appDel.count++;
}


@end
