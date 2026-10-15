#include "ScreenNormilize.h"
#include <iostream>

class CordFGShader: public FGShader{
public:
    using step_t = int;
    using line_size_t = unsigned;

    CordFGShader(const line_size_t& width, const line_size_t& height): width{width}, height{height}
    {}

    bool generateLayer(layer_t& layer, screen_normalize_t& baseScreenNormalize) override{
        using snd = ScreenNormalizeDecard;
        ScreenNormalizeDecard screenNormalize = *dynamic_cast<ScreenNormalizeDecard*>(&baseScreenNormalize);

        
        screen_normalize_t::cord_xy_t point;
        snd::component_xy_t x, y;


        snd::cord_xy_t cord_ij_00 = screenNormalize.baseXYCordXY(0, 0, layer);
        if (screenNormalize.getMinX() <= 0 && 0 <= screenNormalize.getMaxX()){
            snd::component_t cord_x_0 = screenNormalize.getIJcordXY(0, cord_ij_00.second, layer).first;

            snd::component_t start = std::max(0, stepX(0) + cord_x_0);
            snd::component_t end   = std::min(static_cast<step_t>(layer.getWidthPixelUShort()), stepX(width) + cord_x_0);

            for( snd::component_t n = start; n < end; ++n){
                for(snd::component_t i = 0; i < layer.getHeightPixelUShort(); ++i){
                    layer_t::element_t& pixel = layer.getPixel(i, n);
                    point = screenNormalize.getXYCordXY(i, n, layer);
                    pixel.setKchar("\u2016");
                }
            } 
            
            if(screenNormalize.getMaxY() > 0){
                snd::component_t arrow_h = std::min(static_cast<line_size_t>(layer.getHeightPixelUShort()), width * 2);
                for(snd::component_t h = 0; h < arrow_h; ++h){
                    start = std::max(0, -h + cord_x_0);
                    end   = std::min(static_cast<step_t>(layer.getWidthPixelUShort()), h + cord_x_0 + 1);

                    for(snd::component_t j = std::max(0, -arrow_h + cord_x_0); j < start; ++j){
                        layer_t::element_t& pixel = layer.getPixel(h, j);
                        point = screenNormalize.getXYCordXY(h, j, layer);
                        if(pixel.getKchar() == "\u2016")
                            pixel.setKchar(" ");
                    }
                    for(snd::component_t j = start; j < end; ++j){
                        layer_t::element_t& pixel = layer.getPixel(h, j);
                        point = screenNormalize.getXYCordXY(h, j, layer);
                        pixel.setKchar("\u2016");
                    }
                    for(snd::component_t j = end; j < std::min(static_cast<step_t>(layer.getWidthPixelUShort()), arrow_h + cord_x_0 + 1); ++j){
                        layer_t::element_t& pixel = layer.getPixel(h, j);
                        point = screenNormalize.getXYCordXY(h, j, layer);
                        if(pixel.getKchar() == "\u2016")
                            pixel.setKchar(" ");
                    }
                }
            }
        }

        if (screenNormalize.getMinY() <= 0 && 0 <= screenNormalize.getMaxY()){
            snd::component_t cord_y_0 = screenNormalize.baseIJcordXY(cord_ij_00.first, 0, layer).second;

            snd::component_t start = std::max(0, stepY(0) + cord_y_0);
            snd::component_t end   = std::min(static_cast<step_t>(layer.getHeightPixelUShort()), stepY(height) + cord_y_0);

            for( snd::component_t n = start; n < end; ++n){
                for(snd::component_t j = 0; j < layer.getWidthPixelUShort(); ++j){
                    layer_t::element_t& pixel = layer.getPixel(n, j);
                    point = screenNormalize.getXYCordXY(n, j, layer);
                    pixel.setKchar("\u2550");
                }
            }
            
            if(screenNormalize.getMaxX() > 0){
                snd::component_t arrow_w = std::max(static_cast<line_size_t>(0), layer.getWidthPixelUShort() - height * 2);
                for(snd::component_t w = arrow_w; w < layer.getWidthPixelUShort(); ++w){
                    start = std::max(0, cord_y_0 + (w - layer.getWidthPixelUShort()) + 1);
                    end   = std::min(static_cast<step_t>(layer.getHeightPixelUShort()), cord_y_0 - ( w - layer.getWidthPixelUShort()));

                    for(snd::component_t i = std::max(0, cord_y_0 + (arrow_w - layer.getWidthPixelUShort()) + 1); i < start; ++i){
                        layer_t::element_t& pixel = layer.getPixel(i, w);
                        point = screenNormalize.getXYCordXY(i, w, layer);
                        if(pixel.getKchar() == "\u2550")
                            pixel.setKchar(" ");
                    }
                    for(snd::component_t i = start; i < end; ++i){
                        layer_t::element_t& pixel = layer.getPixel(i, w);
                        point = screenNormalize.getXYCordXY(i, w, layer);
                        pixel.setKchar("\u2550");
                    }
                    for(snd::component_t j = end; j < std::min(static_cast<step_t>(layer.getHeightPixelUShort()),  cord_y_0 - (arrow_w - layer.getWidthPixelUShort()) + 1); ++j){
                        layer_t::element_t& pixel = layer.getPixel(j, w);
                        point = screenNormalize.getXYCordXY(j, w, layer);
                        if(pixel.getKchar() == "\u2550")
                            pixel.setKchar(" ");
                    }
                }
            }
        }

        return true;
    }

private:
   
    step_t step(const line_size_t& n, const line_size_t& del){ return -static_cast<step_t>(del /  2) + n; }

    step_t stepX(const line_size_t& n){ return step(n, width); }
    step_t stepY(const line_size_t& n){ return step(n, height); }

    line_size_t width;
    line_size_t height;
};
