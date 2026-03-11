//
//  OpenGLView.m
//  CrossplatformTest
//
//  Created by Gordon Pusnik on 2012-10-29.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#import "OpenGLView.h"

@implementation OpenGLView

@synthesize displayLink = _displayLink;
@synthesize pool = _pool;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:frame options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved |NSTrackingActiveAlways) owner:self userInfo:nil];
        [self addTrackingArea:trackingArea];
    }
    
    return self;
}

- (void)awakeFromNib
{
    NSLog(@"awakeFromNib");
    
    self.pool = nil;
    
    appDelegate = (AppDelegate *)[NSApp delegate];
    
    appDelegate.game = NULL;
    appDelegate.openGLView = self;
    
    NSLog(@"Width: %f, height: %f\n", self.bounds.size.width, self.bounds.size.height);

    [self prepareOpenGL];
    
    appDelegate.game= new Game;
    appDelegate.game->init(self.bounds.size.width, self.bounds.size.height);
    
    // Activate the display link
    CVDisplayLinkStart(_displayLink);
}

- (void)prepareOpenGL
{
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(_displayLink, &MyDisplayLinkCallback, self);
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(_displayLink, cglContext, cglPixelFormat);
}

- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    NSOpenGLContext *currentContext = [self openGLContext];
    [currentContext makeCurrentContext];
    
    // must lock GL context because display link is threaded
    CGLLockContext((CGLContextObj)[currentContext CGLContextObj]);

    // Add your drawing codes here
    if (appDelegate.game) {
        if (! self.pool) {
            self.pool = [[NSAutoreleasePool alloc] init];
        }
        CFTimeInterval time = CFAbsoluteTimeGetCurrent();
        appDelegate.game->update(time);
        appDelegate.game->render();
    }
    
    [currentContext flushBuffer];
    
    CGLUnlockContext((CGLContextObj)[currentContext CGLContextObj]);
    
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime,
                                      CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(OpenGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void)drawRect:(NSRect)dirtyRect
{
}

- (void)keyDown:(NSEvent*)event {
    if (appDelegate.game) {
        appDelegate.game->keyDown([event keyCode]);
    }
}

- (void)keyUp:(NSEvent*)event {
    if (appDelegate.game) {
        appDelegate.game->keyUp([event keyCode]);
    }
}

- (void) mouseDown:(NSEvent*) theEvent
{
    if (appDelegate.game) {
        NSPoint nsPoint = [theEvent locationInWindow];
        nsPoint = [self convertPoint:nsPoint fromView:nil];
        
        float x = nsPoint.x;
        float y = nsPoint.y;

        appDelegate.game->touchesBegan(x, y);
    }

//    [NSCursor hide];
}

- (void) mouseUp:(NSEvent*) theEvent
{
    if (appDelegate.game) {
        NSPoint nsPoint = [theEvent locationInWindow];
        nsPoint = [self convertPoint:nsPoint fromView:nil];
        
        float x = nsPoint.x;
        float y = nsPoint.y;
        
        appDelegate.game->touchesEnded(x, y);
    }
}

- (void) mouseDragged:(NSEvent*) theEvent
{
    if (appDelegate.game) {
        NSPoint nsPoint = [theEvent locationInWindow];
        nsPoint = [self convertPoint:nsPoint fromView:nil];
        
        float x = nsPoint.x;
        float y = nsPoint.y;
        
        appDelegate.game->touchesMoved(x, y);
    }
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)becomeFirstResponder
{
    [[self window] setAcceptsMouseMovedEvents:YES];
    return YES;
}

- (BOOL) resignFirstResponder
{
	//NSLog(@"Resign First Responder");
    [[self window] setAcceptsMouseMovedEvents:NO];
    return YES;
}

@end
