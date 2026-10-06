#include "BlendLayer.h"
#include "FGShader.h"
#include "PixelLayer.h"
#include "Window.h"
#include "engineKonsole"

#include <iostream>


class SampleFGShader1: public FGShader{
public:
    bool generateLayer(layer_t& layer, screen_normalize_t& screenNormalize) override{
        
        screen_normalize_t::cord_xy_t point;
        screen_normalize_t::component_xy_t x, y;
        
        for(layer_t::cord_component_t i = 0; i < layer.getHeightPixelUShort(); ++i){
            for(layer_t::cord_component_t j = 0; j < layer.getWidthPixelUShort(); ++j){
                
                layer_t::element_t& pixel = layer.getPixel(i, j);
                point = screenNormalize.getXYCordXY(i, j, layer);

                point = screenNormalize.moveXYCordXY(point, {0.4, -0.5});

                x = point.first;
                y = point.second;

                if( (x > 0) ? ( (std::log(x) - y) > 0.01 && (std::log(x) - y) < 0.2 ) : false ){
                    pixel.setKchar("*");
                }else{
                    pixel.setKchar(" ");
                }

            }
        }
        return true;
    }
};

class SampleFGShader2: public FGShader{
public:
    bool generateLayer(layer_t& layer, screen_normalize_t& screenNormalize) override{
        
        screen_normalize_t::cord_xy_t point;
        screen_normalize_t::component_xy_t x, y;
        
        for(layer_t::cord_component_t i = 0; i < layer.getHeightPixelUShort(); ++i){
            for(layer_t::cord_component_t j = 0; j < layer.getWidthPixelUShort(); ++j){


                layer_t::element_t& pixel = layer.getPixel(i, j);
                point = screenNormalize.getXYCordXY(i, j, layer);

                point = screenNormalize.rotateXYCordXY(point, 3.1415926535932897 / 180.0 * 30.0);
                
                x = point.first;
                y = point.second;

                if( std::abs(x * x - y * y) > 0.1  && std::abs(x * x - y * y) < 0.2 ){
                    pixel.setKchar("-");
                }else{
                    pixel.setKchar(" ");
                }

            }
        }
        return true;
    }
};

class SampleFGShader3: public FGShader{
public:
    bool generateLayer(layer_t& layer, screen_normalize_t& screenNormalize) override{
        
        screen_normalize_t::cord_xy_t point;
        screen_normalize_t::component_xy_t x, y;

        FGShader::forForIJ(layer, [&](const auto& i, const auto& j){
                layer_t::element_t& pixel = layer.getPixel(i, j);
                point = screenNormalize.getXYCordXY(i, j, layer);

                point = screenNormalize.scaleXYCordXY(point, 10);

                x = point.first;
                y = point.second - 4;

                if( ( y * y - x) > 0.1  && (y * y - x) < 8 ){
                    pixel.setKchar("-");
                }else{
                    pixel.setKchar(" ");
                }
        });
        
        return true;
    }
};

class SampleBlendLayer: public BlendLayer<Pixel, Pixel>{
public:
    SampleBlendLayer(const cord_component_t& width, const cord_component_t& height): BlendLayer{width, height}
    {}

    code_flag_t blendLayer(const layer_t& layer) override {
        for(layer_t::cord_component_t i = 0; i < layer.getHeightPixelUShort(); ++i){
            for(layer_t::cord_component_t j = 0; j < layer.getWidthPixelUShort(); ++j){
                
                const layer_t::element_t& pixel1 = layer.getPixel(i, j);
                layer_t::element_t& pixel2 = frame.getPixel(i, j);

                if(pixel1.getKchar() == "-"){
                    if(pixel2.getKchar() == "*"){
                        pixel2.setKchar("=");
                    }else if(pixel2.getKchar() == "-"){
                        pixel2.setKchar("@");
                    }else if(pixel2.getKchar() == "="){
                        pixel2.setKchar("$");
                    }else{
                        pixel2.setKchar("-");
                    }
                }

            }
        }

        return 0;
    }
};



class SampleWindow: public Window{
public:
    typedef Window super_class_t;

    SampleWindow(cord_component_t width, cord_component_t height): Window(width, height),
        blendLayer(width, height), swapChain(2, width, height)
    {}

    void drawScreen(std::ostream& ost) override {
        SampleFGShader1 sh1;
        SampleFGShader2 sh2;
        SampleFGShader3 sh3;
        
        LayerP& layer1 = swapChain.getLayers()[0];
        LayerP& layer2 = swapChain.getLayers()[1];
        
        if(sh1.generateLayer(layer1, screenNormalize)){
            blendLayer.setFrame(Frame(layer1));
            // blendLayer.getFrame().drawFrame(ost);

            sh2.generateLayer(layer2, screenNormalize);
            blendLayer.blendLayer(layer2);
            
            // blendLayer.fillClear(layer2);
            sh3.generateLayer(layer2, screenNormalize);
            blendLayer.blendLayer(layer2);

            blendLayer.getFrame().drawFrame(ost);
        }
    }

private:
    SwapChain<Pixel> swapChain;

    SampleBlendLayer blendLayer;
    ScreenNormalize  screenNormalize;
};

int main(){
    SampleWindow window(50, 50);
    window.drawScreen(std::cout);

    return 0;
}
