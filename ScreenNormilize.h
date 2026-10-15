#include "FGShader.h"

struct ScreenNormalizeDecard: public ScreenNormalize{
    using layer_t = ScreenNormalize::layer_t;
    using cord_t =  ScreenNormalize::cord_t;
    using component_t = ScreenNormalize::component_t;
    using component_xy_t = ScreenNormalize::component_xy_t;
    using cord_xy_t = ScreenNormalize::cord_xy_t;
    using cord_ij_t = std::pair<component_t, component_t>;
    using angle_t = ScreenNormalize::angle_t;

    ScreenNormalizeDecard(
            const component_xy_t& worldCordX = 0.0,
            const component_xy_t& worldCordY = 0.0,
            const component_xy_t& scaleX = 2.0,
            const component_xy_t& scaleY = 2.0,
            const component_xy_t& symX   = 1.0,
            const component_xy_t& symY   = 1.0
    ): scaleX{scaleX}, scaleY{scaleY}, symX{symX}, symY{symY}, worldCordX{worldCordX}, worldCordY{worldCordY}
    {}

    cord_xy_t baseXYCordXY(const component_t& i, const component_t& j, const layer_t& baseLayer) const{
        return cord_xy_t(
            (static_cast<component_xy_t>(j) / baseLayer.getWidthPixelUShort()) * scaleX - symX + worldCordX,
            -(static_cast<component_xy_t>(i) / baseLayer.getHeightPixelUShort()) * scaleY + symY + worldCordY
        );    
    }

    cord_ij_t baseIJcordXY(const component_xy_t& x, const component_xy_t& y, const layer_t& baseLayer) const{
        return cord_ij_t(
            (x + symX - worldCordX) / scaleX  * baseLayer.getWidthPixelUShort(),
            (y - symY - worldCordY)  / -scaleY * baseLayer.getHeightPixelUShort()
        );
    }

    const component_xy_t getMaxX() const noexcept{ return (1.0) * scaleX - symX + worldCordX; }
    const component_xy_t getMinX() const noexcept{ return (0.0) * scaleX - symX + worldCordX; }

    const component_xy_t getMaxY() const noexcept{ return -(0.0) * scaleY + symY + worldCordY; }
    const component_xy_t getMinY() const noexcept{ return -(1.0) * scaleY + symY + worldCordY; }

    virtual cord_xy_t getXYCordXY(const component_t& i, const component_t& j, const layer_t& layer) const { return baseXYCordXY(i, j, layer); }
    virtual cord_ij_t getIJcordXY(const component_xy_t& x, const component_xy_t& y,  const layer_t& baseLayer) const{ return baseIJcordXY(x, y, baseLayer); }

    const component_xy_t& getScaleX() const noexcept { return scaleX; }
    const component_xy_t& getScaleY() const noexcept { return scaleY; }

    const component_xy_t& getSymX() const noexcept { return symX; }
    const component_xy_t& getSymY() const noexcept { return symY; }

    const component_xy_t& getWorldCordX() const noexcept { return worldCordX; }
    const component_xy_t& getWorldCordY() const noexcept { return worldCordY; }

protected: 

    const component_xy_t scaleX;
    const component_xy_t scaleY;
    
    const component_xy_t symX;
    const component_xy_t symY;

    const component_xy_t worldCordX; 
    const component_xy_t worldCordY; 
};
