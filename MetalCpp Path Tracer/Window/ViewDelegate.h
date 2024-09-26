#ifndef VIEW_DELEGATE_H
#define VIEW_DELEGATE_H

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "Renderer.h"

namespace MetalCppPathTracer
{

class ViewDelegate : public MTK::ViewDelegate
{
 public:
    ViewDelegate(MTL::Device *pDevice);
    virtual ~ViewDelegate() override;
    virtual void drawInMTKView(MTK::View* pView) override;
    virtual void drawableSizeWillChange(MTK::View *pView, CGSize size) override;

 private:
    Renderer* _pRenderer;
};

};

#endif  //  VIEW_DELEGATE_H
