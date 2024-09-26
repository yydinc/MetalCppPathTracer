#ifndef APPLICATION_DELEGATE_H
#define APPLICATION_DELEGATE_H

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>

#include "ViewDelegate.h"

namespace MetalCppPathTracer
{

class ApplicationDelegate : public NS::ApplicationDelegate
{
    public:
        ~ApplicationDelegate();

        virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
        virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
        virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

    private:
        NS::Window* _pWindow;
        MTK::View* _pMtkView;
        MTL::Device* _pDevice;
        ViewDelegate* _pViewDelegate = nullptr;
};

};

#endif  //  APPLICATION_DELEGATE_H
