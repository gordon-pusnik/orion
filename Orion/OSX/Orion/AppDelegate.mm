//
//  AppDelegate.m
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-29.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#import "AppDelegate.h"
#import "OpenGLView.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize game = _game;
@synthesize openGLView = _openGLView;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    [[self window] makeKeyAndOrderFront:self];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    if (_openGLView) {
        CVDisplayLinkStop(_openGLView.displayLink);
        CVDisplayLinkRelease(_openGLView.displayLink);
    }
}

- (void)applicationDidTerminate:(NSNotification *)aNotification
{
    if (_game) {
        delete _game;
    }
    
    if (_openGLView) {
        [_openGLView.pool release];
    }
}

@end
