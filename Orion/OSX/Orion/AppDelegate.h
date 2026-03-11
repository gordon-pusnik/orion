//
//  AppDelegate.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-29.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Game.h"

@class OpenGLView;

@interface AppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *_window;
    Game *_game;
    OpenGLView *_openGLView;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) Game *game;
@property (assign) OpenGLView *openGLView;

@end
