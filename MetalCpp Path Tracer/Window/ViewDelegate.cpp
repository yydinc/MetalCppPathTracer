#include "ViewDelegate.h"

using namespace MetalCppPathTracer;

ViewDelegate::ViewDelegate( MTL::Device* pDevice )
: MTK::ViewDelegate()
, _pRenderer(new Renderer(pDevice))
{
}

ViewDelegate::~ViewDelegate()
{
    delete _pRenderer;
}

void ViewDelegate::drawInMTKView( MTK::View* pView )
{
    _pRenderer->draw(pView);
}

void ViewDelegate::drawableSizeWillChange(MTK::View *pView, CGSize size)
{
    _pRenderer->drawableSizeWillChange(pView, size);
}
