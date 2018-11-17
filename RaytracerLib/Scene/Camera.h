#pragma once

#include "../RayLib.h"

#include "../Math/Transform.h"
#include "../Math/Ray.h"
#include "../Math/Random.h"
#include "../Math/Simd8Ray.h"


namespace rt {

namespace math {
class Vector2x8;
}

struct RenderingContext;

enum class BokehShape : Uint8
{
    Circle = 0,
    Hexagon,
    Square,
};

/**
 * Depth of Field settings.
 */
struct DOFSettings
{
    // distance from camera at which plane of perfect focus is located
    Float focalPlaneDistance;

    // the bigger value, the bigger out-of-focus blur
    Float aperture;

    BokehShape bokehType;

    DOFSettings()
        : focalPlaneDistance(2.0f)
        , aperture(0.02f)
        , bokehType(BokehShape::Circle)
    { }
};


/**
 * Class describing camera for scene raytracing.
 */
class RT_ALIGN(16) RAYLIB_API Camera
{
public:
    Camera();

    void SetPerspective(const math::Transform& transform, Float aspectRatio, Float FoV);

    void SetAngularVelocity(const math::Quaternion& quat);

    // Sample camera transfrom for given time point
    RT_FORCE_INLINE math::Transform SampleTransform(const float time) const;

    // Generate ray for the camera for a given time
    // x and y coordinates should be in [0.0f, 1.0f) range.
    math::Ray GenerateRay(const math::Vector4 coords, RenderingContext& context) const;
    math::Ray_Simd8 GenerateRay_Simd8(const math::Vector2x8& coords, RenderingContext& context) const;

    RT_FORCE_INLINE math::Vector4 GenerateBokeh(RenderingContext& context) const;

    // TODO generate ray packet

    // camera placement
    math::Transform mTransform;

    // camera velocity
    math::Vector4 mLinearVelocity = math::Vector4::Zero();

    // width to height ratio
    Float mAspectRatio;

    // in radians, vertical angle
    Float mFieldOfView;

    // depth of field settings
    DOFSettings mDOF;

    // camera lens distortion (0.0 - no distortion)
    Float barrelDistortionConstFactor;
    Float barrelDistortionVariableFactor;
    Bool enableBarellDistortion;

private:
    Float mTanHalfFoV;

    math::Quaternion mAngularVelocity;
    bool mAngularVelocityIsZero;
};

} // namespace rt