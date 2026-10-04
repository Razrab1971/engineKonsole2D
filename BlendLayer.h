#pragma once

#include "PixelLayer.h"

#include <functional>
#include <ostream>


typedef Layer<Pixel> LayerP;


class Frame: public LayerP{
public:
    typedef LayerP layer_t;

    Frame(const layer_t::cord_component_t& width, const layer_t::cord_component_t& height): LayerP(width, height) 
    {}
    Frame(const LayerP& layer): LayerP(layer) 
    {}

    Frame& operator=(const layer_t& rhs){
        if (static_cast<const layer_t*>(this) == &rhs) {
            return *this;
        }
        layer_t::operator=(rhs);
        return *this;
    }

    virtual ~Frame(){}
    virtual bool drawFrame(std::ostream& ost){
        for(size_t i = 0; i < layer_t::windowSize[1]; ++i){
            for(size_t j = 0; j < layer_t::windowSize[0]; ++j){
                ost << layer_t::getPixel(i,j).getKcharStdStrind();
            }
            ost << std::endl;
        }
        return true;
    }
};


template<typename Element>
class BlendLayer{
public:
    typedef Layer<Element> layer_t;
    typedef Frame frame_t;
    typedef Layer<Element>::cord_component_t cord_component_t;
    typedef char code_flag_t;
    
    void fillClear(std::function<void(Pixel&)> f = [](Pixel& p) {p.setKcharStdStrind(" "); }) {
        for(cord_component_t i = 0; i < frame.getWidthPixelUShort(); ++i)
            for(cord_component_t j = 0; j < frame.getHeightPixelUShort(); ++j)
                f(frame.getPixel(i, j));
    }

    virtual code_flag_t blendLayer(const layer_t& layer){
        frame = layer;
        return 0;
    }

    BlendLayer(frame_t&& layer): frame(layer)
    {}
    BlendLayer(const cord_component_t& width, const cord_component_t& height): frame{width, height}
    {}

    virtual ~BlendLayer(){}
    virtual frame_t getFrame(){ return frame; }
    void setFrame(const frame_t& frame){ this->frame = frame;}
protected:
    frame_t frame;
};
