#import "ControllerView.hpp"
#import <MetalKit/MetalKit.h>

#include "InputSystem.h"

@interface ViewBridge : MTKView {
}
+ (void)load:(CGRect)frame;
+ (ViewBridge *)get;
@end

ViewBridge *adapter;

MTK::View *MetalCppPathTracer::ControllerView::get(CGRect frame) {
    [ViewBridge load: frame];
    return (__bridge MTK::View *)[ViewBridge get];
}

@implementation ViewBridge

+ (void)load:(CGRect)frame {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    adapter = [[self alloc] initWithFrame:frame];
    [adapter init];
    [pool release];
}

+ (ViewBridge *)get {
    return adapter;
}

- (id)init {
    [self becomeFirstResponder];
    return self;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)mouseDragged:(NSEvent *)event {
    MetalCppPathTracer::InputSystem::rotationInput.x = [event deltaX];
    MetalCppPathTracer::InputSystem::rotationInput.y = [event deltaY];
}

- (void)keyDown:(NSEvent *)event {
    if(event.keyCode == 2) MetalCppPathTracer::InputSystem::movementInput.x = 1; // right - d
    else if(event.keyCode == 0) MetalCppPathTracer::InputSystem::movementInput.x = -1; // left - a
    
    if(event.keyCode == 49) MetalCppPathTracer::InputSystem::movementInput.y = 1; // up - space
    else if(event.keyCode == 8) MetalCppPathTracer::InputSystem::movementInput.y = -1; // down - c
    
    if(event.keyCode == 13) MetalCppPathTracer::InputSystem::movementInput.z = 1; // forward - w
    else if(event.keyCode == 1) MetalCppPathTracer::InputSystem::movementInput.z = -1; // backward - s
    
    if(event.keyCode == 15) MetalCppPathTracer::InputSystem::resetInput = 1;
}

- (void)keyUp:(NSEvent *)event {
    
    if(event.keyCode == 2) MetalCppPathTracer::InputSystem::movementInput.x = 0; // right - d
    else if(event.keyCode == 0) MetalCppPathTracer::InputSystem::movementInput.x = 0; // left - a
    
    if(event.keyCode == 49) MetalCppPathTracer::InputSystem::movementInput.y = 0; // up - space
    else if(event.keyCode == 8) MetalCppPathTracer::InputSystem::movementInput.y = 0; // down - c
    
    if(event.keyCode == 13) MetalCppPathTracer::InputSystem::movementInput.z = 0; // forward - w
    else if(event.keyCode == 1) MetalCppPathTracer::InputSystem::movementInput.z = 0; // backward - s
}

-(void)scrollWheel:(NSEvent *)event {
    MetalCppPathTracer::InputSystem::zoomInput = -event.scrollingDeltaY;
}

@end
