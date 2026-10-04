#pragma once

#include <cstring>
#include <string>
#include <sys/types.h>
#include <stdexcept>
#include <vector>

// Хранит в себе объект: { цвет, основной символ }
// Кордината появляется в Layer
class Pixel{
public:
    // typedef short cord_t[2]; // Координата
    typedef char color_t[4];    // Цвет
    typedef std::string sequetion_unicode_t; //Основной символ  
    
    Pixel(): color{0, 0, 0, 0}, kchar("")
    {}
    Pixel(const color_t& color): kchar("@")
    {
        setColor(color);
    }
    Pixel(const color_t& color, const sequetion_unicode_t& kchar): kchar(kchar)
    {
        setColor(color);
    }

    Pixel(const char& r, const char& g, const char& b, const char& a, char ch):color{r, g, b, a},  kchar(std::to_string(ch))
    {}

    Pixel(const Pixel& rhs){
        setKchar(rhs.kchar);
        setColor(rhs.color);
    }

    Pixel(Pixel&& rhs){
        std::swap(color, rhs.color);
        kchar = std::move(rhs.kchar);
    }

    Pixel& operator=(const Pixel& rhs) {
    if (this != &rhs) {
        setKchar(rhs.kchar);
        setColor(rhs.color);
    }
    return *this;
    }


    Pixel& operator=(Pixel&& rhs) noexcept {
        if (this != &rhs) {
            std::swap_ranges(std::begin(color), std::end(color), std::begin(rhs.color));
            kchar = std::move(rhs.kchar);
        }
        return *this;
    }



    // const cord_t& getCordShort2() const noexcept{ return cord; }
    // void setCordShort2(const cord_t& cord) noexcept{ std::memcpy(this->cord, cord, sizeof(this->cord)); }
    // void setCordShort2(const short& x, const short& y) noexcept{ cord[0] = x; cord[1] = y; }
    // cord_t& getCordShort2() noexcept{ return cord; }

    void setColorChar4(const char& r, const char& g, const char& b, const char& a) noexcept{ color[0] = r; color[1] = g; color[2] = b; color[3] = a; }
    const char* getColorChar4() const noexcept{ return color; }
    char* getColorChar4() noexcept{ return color; }

    void setColor(const color_t& color) noexcept{ std::memcpy(this->color, color, sizeof(this->color)); }
    const color_t& getColor() const noexcept{return color;}
    color_t& getColor() noexcept{return color;}
    
    void setKcharStdStrind(const std::string& str) noexcept{ kchar = str;}
    const std::string& getKcharStdStrind() const noexcept{ return kchar; }
    std::string& getKcharStdStrind() noexcept{ return kchar; }

    void setKchar(const sequetion_unicode_t& kchar){ this->kchar = kchar; }
    const sequetion_unicode_t& getKchar() const noexcept{ return kchar; }
    sequetion_unicode_t& getKchar() noexcept{ return kchar;}
private:
    // cord_t cord;
    color_t color;
    sequetion_unicode_t kchar;
};

template<typename Element>
class Layer{
public:
    typedef std::vector<Element> layer_t;
    typedef Element element_t;
    typedef unsigned short cord_component_t;
    typedef std::pair<cord_component_t, cord_component_t> cord_t;
    typedef cord_component_t window_size_t[2];

    Layer(const window_size_t& size): windowSize{size[0], size[1]}, layer(size[0] * size[1])
    {}
    Layer(const unsigned short width, const unsigned short height): windowSize{width, height}, layer(width * height)
    {}

    Layer(const Layer& rhs): windowSize{rhs.windowSize[0], rhs.windowSize[1]}, layer(rhs.layer)
    {}
    Layer(Layer&& rhs): windowSize{rhs.windowSize[0], rhs.windowSize[1]}, layer(std::move(rhs.layer))
    {}

    Layer& operator=(const Layer& rhs){
        if (this == &rhs) {
            return *this;
        }
        
        if (this->windowSize[0] != rhs.windowSize[0] || this->windowSize[1] != rhs.windowSize[1]) {
            throw std::invalid_argument("Ошибка: Размеры слоев не совпадают для операции присваивания!");
        }
        layer = rhs.layer;
        return *this;
    }

    const unsigned short* getWindowSizeUShort2() const noexcept{return windowSize;}
    const unsigned short getWidthPixelUShort() const noexcept{return windowSize[0];}
    const unsigned short getHeightPixelUShort() const noexcept{return windowSize[1];}
    
    element_t& getPixel(const cord_component_t& i, const cord_component_t& j){
        return layer[i * getWidthPixelUShort() + j];
    }

    element_t& getPixel(const cord_t& cord){
        return layer[cord.first * getWidthPixelUShort() + cord.second];
    }

    const element_t& getPixel(const cord_component_t& i, const cord_component_t& j) const{
        return layer[i * getWidthPixelUShort() + j];
    }

    const element_t& getPixel(const cord_t& cord) const{
        return layer[cord.first * getWidthPixelUShort() + cord.second];
    }

    bool isPixelInLayer(const cord_component_t& i, const cord_component_t& j ) const noexcept{
        return i < windowSize[0] && j < windowSize[1]; 
    }

protected:
    const window_size_t windowSize;
    layer_t layer;
};

template<typename Element>
class SwapChain{
public:
    typedef Layer<Element> layer_t;
    typedef std::vector<layer_t> layers_t;
    typedef layer_t::cord_component_t cord_component_t;
        
    SwapChain(size_t count, cord_component_t width, cord_component_t height):
        layers(count, layer_t(width, height))
    {}
    
    layers_t& getLayers(){ return layers; }
    size_t getSize(){return layers.size(); }

private:
    layers_t layers;
};
