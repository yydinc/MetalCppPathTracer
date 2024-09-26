#include "ApplicationDelegate.h"
#include "ControllerView.hpp"

using namespace MetalCppPathTracer;

ApplicationDelegate::~ApplicationDelegate()
{
    _pMtkView->release();
    _pWindow->release();
    _pDevice->release();
    delete _pViewDelegate;
}

void ApplicationDelegate::applicationWillFinishLaunching( NS::Notification* pNotification )
{
    NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
    pApp->setActivationPolicy( NS::ActivationPolicy::ActivationPolicyRegular );
}

void ApplicationDelegate::applicationDidFinishLaunching( NS::Notification* pNotification )
{
    CGRect frame = (CGRect){{100.0, 100.0}, {1280.0, 720.0}};

    _pWindow = NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable|NS::WindowStyleMaskTitled|NS::WindowStyleMaskResizable,
        NS::BackingStoreBuffered,
        false );

    _pDevice = MTL::CreateSystemDefaultDevice();
    
    _pMtkView = MTK::View::alloc()->init( frame, _pDevice );
    
    ControllerView v;
    
    _pMtkView = v.get(frame);
    
    _pMtkView->setDevice(_pDevice);
    
    _pMtkView->setColorPixelFormat( MTL::PixelFormat::PixelFormatRGBA16Float);
    _pMtkView->setClearColor( MTL::ClearColor::Make( 1.0, 0.0, 0.0, 1.0 ) );

    _pViewDelegate = new ViewDelegate( _pDevice );
    _pMtkView->setDelegate( _pViewDelegate );

    _pWindow->setContentView( _pMtkView );
    _pWindow->setTitle( NS::String::string( "MetalCpp Path Tracer", NS::StringEncoding::UTF8StringEncoding ) );

    _pWindow->makeKeyAndOrderFront( nullptr );

    NS::Application* pApp = reinterpret_cast< NS::Application* >( pNotification->object() );
    pApp->activateIgnoringOtherApps( true );
}

bool ApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender )
{
    return true;
}
