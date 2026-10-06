#pragma once

#include "PixelLayer.h"
#include <cmath>
#include <functional>
#include <utility>


typedef Layer<Pixel> LayerP;

// Преобразование (i, j) -> (x, y)
struct ScreenNormalize{
    using layer_t = LayerP;
    using cord_t =  layer_t::cord_t;
    using component_t = layer_t::cord_component_t;
    using component_xy_t = double;
    using cord_xy_t = std::pair<component_xy_t, component_xy_t>;
    using angle_t = double;

    virtual ~ScreenNormalize(){}

    cord_xy_t baseXYCordXY(const component_t& i, const component_t& j, const layer_t& baseLayer) const{
        return cord_xy_t(
            (static_cast<component_xy_t>(j) / baseLayer.getWidthPixelUShort()) * 2.0 - 1.0,
            -(static_cast<component_xy_t>(i) / baseLayer.getHeightPixelUShort()) * 2.0 + 1.0
        );    
    }

    cord_xy_t moveXYCordXY(const cord_xy_t& cord, const cord_xy_t& worldPoint){
        return cord_xy_t(
                cord.first + worldPoint.first,
                cord.second + worldPoint.second
        );    
    }

    cord_xy_t rotateXYCordXY(const cord_xy_t cord, const angle_t& angle){
        component_xy_t c_a = std::cos(angle);
        component_xy_t s_a = std::sin(angle);

        return cord_xy_t(
                cord.first * c_a + cord.second * s_a,
                -cord.first * s_a + cord.second * c_a
        );    
    }

    cord_xy_t scaleXYCordXY(const cord_xy_t cord, const cord_xy_t scale){
        return cord_xy_t(
            cord.first * scale.first,
            cord.second * scale.second
        );    
    }

    cord_xy_t scaleXYCordXY(const cord_xy_t cord, component_xy_t scale){
        return cord_xy_t(
            cord.first * scale,
            cord.second * scale
        );    
    }

    virtual cord_xy_t getXYCordXY(const component_t& i, const component_t& j, const layer_t& layer) const { return baseXYCordXY(i, j, layer); }
    cord_xy_t getXYCordXY(const cord_t& cord, const layer_t& layer) const {return getXYCordXY(cord.first, cord.second, layer); } 
};


class FGShader{
public:
    typedef ScreenNormalize screen_normalize_t;
    typedef LayerP layer_t;
    
    virtual bool generateLayer(layer_t& layer, screen_normalize_t& screenNormalize){ return false; } 
    virtual ~FGShader(){}

    void forForIJ(const layer_t& layer, std::function<void(const layer_t::cord_component_t& i ,const layer_t::cord_component_t& j)> func){
        for(layer_t::cord_component_t i = 0; i < layer.getHeightPixelUShort(); ++i){
            for(layer_t::cord_component_t j = 0; j < layer.getWidthPixelUShort(); ++j){
                func(i, j);
            }
        }
    }
};
