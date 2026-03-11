//
//  ViewController.m
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-29.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#import "ViewController.h"
#import "Game.h"

@interface ViewController () {
    Game *_game;
    
    UIPinchGestureRecognizer *pinchRecognizer;
    UIPanGestureRecognizer *panRecognizer;
    UIRotationGestureRecognizer *rotationRecognizer;
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation ViewController

- (void)dealloc
{
    [self tearDownGL];
    
    if (_game) {
        delete _game;
    }
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [_context release];

    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
/*
    UIPinchGestureRecognizer *pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinch:)];
	pinchRecognizer.delegate = self;
    [self.view addGestureRecognizer:pinchRecognizer];
    [pinchRecognizer release];
    
	UIPanGestureRecognizer *panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
	panRecognizer.delegate = self;
    [panRecognizer setMinimumNumberOfTouches:2];
    [panRecognizer setMaximumNumberOfTouches:2];
    [self.view addGestureRecognizer:panRecognizer];
    [panRecognizer release];
	
	UIRotationGestureRecognizer *rotationRecognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotate:)];
	rotationRecognizer.delegate = self;
    [self.view addGestureRecognizer:rotationRecognizer];
    [rotationRecognizer release];
*/
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];
    self.preferredFramesPerSecond = 60;

    if (! self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    _game = NULL;
    
    [self setupGL];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    bool retina = false;
    if ([UIScreen mainScreen].scale == 2.0) {
        retina = true;
    }
    
    _game = new Game;
    _game->init(self.view.bounds.size.width, self.view.bounds.size.height, retina);
    _game->setViewController(self);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
//    NSLog(@"Touch began");

//    UITouch * touch = [touches anyObject];
    for (UITouch* touch in [touches allObjects]) {
        CGPoint location = [touch locationInView:self.view];
        
        if (_game) {
            _game->touchesBegan(location.x, self.view.bounds.size.height - location.y);
        }
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
//    NSLog(@"Touch moved");

//    UITouch * touch = [touches anyObject];
    for (UITouch* touch in [touches allObjects]) {
        CGPoint location = [touch locationInView:self.view];
        
        if (_game) {
            _game->touchesMoved(location.x, self.view.bounds.size.height - location.y);
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
//    NSLog(@"Touch ended");
    
//    UITouch * touch = [touches anyObject];
    for (UITouch* touch in [touches allObjects]) {
        CGPoint location = [touch locationInView:self.view];
        
        if (_game) {
            _game->touchesEnded(location.x, self.view.bounds.size.height - location.y);
        }
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
//    NSLog(@"Touch cancelled");

    for (UITouch* touch in [touches allObjects]) {
        CGPoint location = [touch locationInView:self.view];
        
        if (_game) {
            _game->touchesEnded(location.x, self.view.bounds.size.height - location.y);
        }
    }
}

-(void)pinch:(UIPinchGestureRecognizer*)gesture
{
	if ([gesture state] == UIGestureRecognizerStateBegan) {        
        if (_game) {
            _game->pinchBegan();
        }
	}
	else if ([gesture state] == UIGestureRecognizerStateChanged) {
        float scale = gesture.scale;
        
        if (_game) {
            _game->pinchChanged(scale);
        }
    }
	else if ([gesture state] == UIGestureRecognizerStateEnded) {        
        if (_game) {
            _game->pinchEnded();
        }
	}
}

-(void)pan:(UIPanGestureRecognizer*)gesture {
    if ([gesture state] == UIGestureRecognizerStateBegan) {
//		panInitial = CGPointMake(0, 0);
//		panTimePassed=1;
        if (_game) {
            _game->panBegan();
        }
	}
	if ([gesture state] == UIGestureRecognizerStateChanged) {
        CGPoint translation = [gesture translationInView:[gesture view]];
//		[self rotateQuaternionWithVector:CGPointMake(translation.x-panInitial.x, -(translation.y-panInitial.y))];
//		panInitial = translation;
        if (_game) {
            _game->panChanged(translation.x, translation.y);
        }
    }
	if ([gesture state] == UIGestureRecognizerStateEnded) {
//		panVelocity = [gesture velocityInView:[gesture view]];
//		panTimePassed=0;
        if (_game) {
            _game->panEnded();
        }
	}
}

-(void)rotate:(UIRotationGestureRecognizer*)gesture {
    if ([gesture state] == UIGestureRecognizerStateBegan) {
//		rotateInitial = 0;
//		rotateTimePassed=1;
        if (_game) {
            _game->rotateBegan();
        }
	}
	if ([gesture state] == UIGestureRecognizerStateChanged) {
//		userQuarternion = GLKQuaternionMultiply(GLKQuaternionMakeWithAngleAndAxis(gesture.rotation-rotateInitial, 0, 0, -1),userQuarternion);
//		rotateInitial = gesture.rotation;
        
        if (_game) {
            _game->rotateChanged(gesture.rotation);
        }
    }
	if ([gesture state] == UIGestureRecognizerStateEnded) {
//		rotateVelocity = gesture.velocity;
//		rotateTimePassed=0;
        if (_game) {
            _game->rotateEnded();
        }
	}
}

-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch{
	if ([gestureRecognizer isKindOfClass:[UITapGestureRecognizer class]] && [touch.view.superview isKindOfClass:[GLKView class]]) return FALSE;
	return TRUE;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
	if ([gestureRecognizer isKindOfClass:[UITapGestureRecognizer class]] || [otherGestureRecognizer isKindOfClass:[UITapGestureRecognizer class]]) return NO;
    return YES;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    if (_game) {
        CFTimeInterval time = CFAbsoluteTimeGetCurrent();
        _game->update(time);
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if (_game) {
        _game->render();
    }
}

- (void) enableMultiTouch:(BOOL) b
{
    [self.view setMultipleTouchEnabled:b];
    
    if (b) {
        [self.view removeGestureRecognizer:pinchRecognizer];
        [self.view removeGestureRecognizer:panRecognizer];
        [self.view removeGestureRecognizer:rotationRecognizer];
    }
    else {
        pinchRecognizer = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinch:)];
        pinchRecognizer.delegate = self;
        [self.view addGestureRecognizer:pinchRecognizer];
        [pinchRecognizer release];
        
        panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(pan:)];
        panRecognizer.delegate = self;
        [panRecognizer setMinimumNumberOfTouches:2];
        [panRecognizer setMaximumNumberOfTouches:2];
        [self.view addGestureRecognizer:panRecognizer];
        [panRecognizer release];
        
        rotationRecognizer = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotate:)];
        rotationRecognizer.delegate = self;
        [self.view addGestureRecognizer:rotationRecognizer];
        [rotationRecognizer release];        
    }
}

@end
