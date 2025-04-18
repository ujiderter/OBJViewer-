#ifndef OBJVIEWER_LIGHT_H
#define OBJVIEWER_LIGHT_H

#include "../model/math/Vector3D.h"

enum class LightType {
    Directional,
    Point,
    Spot
};

class Light {
public:
    LightType type;
    VecMath::Vector3D<float> position;
    VecMath::Vector3D<float> direction;
    VecMath::Vector3D<float> color; 
    float intensity;

    Light(LightType type,
          const VecMath::Vector3D<float>& position,
          const VecMath::Vector3D<float>& direction,
          const VecMath::Vector3D<float>& color,
          float intensity)
        : type(type),
          position(position),
          direction(direction.normalized()),
          color(color),
          intensity(intensity) {}

    [[nodiscard]] VecMath::Vector3D<float> getPosition() const { return position; }
    [[nodiscard]] VecMath::Vector3D<float> getDirection() const { return direction; }
    [[nodiscard]] VecMath::Vector3D<float> getColor() const { return color; }
    [[nodiscard]] float getIntensity() const { return intensity; }

    void setPosition(const VecMath::Vector3D<float>& pos) { position = pos; }
    void setDirection(const VecMath::Vector3D<float>& dir) { direction = dir.normalized(); }
    void setColor(const VecMath::Vector3D<float>& col) { color = col; }
    void setIntensity(float intens) { intensity = std::clamp(intens, 0.0f, 1.0f); }
};

#endif // OBJVIEWER_LIGHT_H