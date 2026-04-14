//
//  OpenGLView.h
//  CrossplatformTest
//
//  Created by Gordon Pusnik on 2012-10-29.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

@interface OpenGLView : NSOpenGLView {
    AppDelegate *appDelegate;
    CVDisplayLinkRef _displayLink;
    NSAutoreleasePool *_pool;
}

@property (assign) CVDisplayLinkRef displayLink;
@property (assign) NSAutoreleasePool *pool;

- (NSSize)drawableSize;

@end
