#pragma once

#include "FGShader.h"
#include "BlendLayer.h"
#include "PixelLayer.h"

class Window{
public:
    typedef LayerP::cord_component_t cord_component_t;

    Window(cord_component_t width, cord_component_t height): width{width}, height{height}
    {}
    
    virtual ~Window(){}
    virtual void drawScreen(std::ostream& ost) = 0;
protected:
    cord_component_t width;
    cord_component_t height;
};
