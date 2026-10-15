#include "engineKonsole"
#include "CordFGShader.h"

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

                // point = screenNormalize.moveXYCordXY(point, {0.4, -0.5});

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


class SampleWindow: public Window{
public:
    typedef Window super_class_t;

    SampleWindow(cord_component_t width, cord_component_t height): Window(width, height),
        blendLayer(width, height), swapChain(2, width, height)
    {}

    void drawScreen(std::ostream& ost) override {
        SampleFGShader1 sh1;
        CordFGShader    shCord(3, 2);

        LayerP& layer1 = swapChain.getLayers()[0];
        
        if(sh1.generateLayer(layer1, screenNormalize)){
            shCord.generateLayer(layer1, screenNormalize);
            blendLayer.setFrame(Frame(layer1));
            blendLayer.getFrame().drawFrame(ost);
        }
    }

private:
    SwapChain<Pixel> swapChain;

    BlendLayer<Pixel, Pixel> blendLayer;
    ScreenNormalizeDecard screenNormalize{-2, -0.4};
};

int main(){
    SampleWindow window(500, 125);
    window.drawScreen(std::cout);

    return 0;
}
