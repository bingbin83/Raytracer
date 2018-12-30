#pragma once

#include "../../RayLib.h"
#include "../../Rendering/ShadingData.h"
#include "../../Utils/AlignmentAllocator.h"
#include "../../Math/Ray.h"
#include "../../Color/Color.h"

namespace rt {

class Material;

namespace math
{
    class Random;
}

// Abstract class for Bidirectional Scattering Density Function
// Handles both reflection and transmission
// NOTE: all the calculations are performed in local-space of the hit point on a surface: X is tangent, Z is normal
// NOTE: it's not called "BRDF", because it handles transmission as well
class BSDF : public Aligned<16>
{
public:
    enum EventType : Uint8
    {
        NullEvent                   = 0,
        DiffuseReflectionEvent      = 1 << 0,
        DiffuseTransmissionEvent    = 1 << 1,
        GlossyReflectionEvent       = 1 << 2,
        GlossyRefractionEvent       = 1 << 3,
        SpecularReflectionEvent     = 1 << 4,
        SpecularRefractionEvent     = 1 << 5,

        DiffuseEvent                = DiffuseReflectionEvent | DiffuseTransmissionEvent,
        GlossyEvent                 = GlossyReflectionEvent | GlossyRefractionEvent,
        SpecularEvent               = SpecularReflectionEvent | SpecularRefractionEvent,

        ReflectiveEvent             = DiffuseReflectionEvent | GlossyReflectionEvent | SpecularReflectionEvent,
        TransmissiveEvent           = SpecularRefractionEvent | GlossyRefractionEvent | DiffuseTransmissionEvent,

        AnyEvent                    = ReflectiveEvent | TransmissiveEvent,
    };

    // If incoming/outgoing direction is at extremely grazing angle, the BSDF will early-return zero value
    // in order to avoid potential divisions by zero.
    static constexpr Float CosEpsilon = 1.0e-5f;

    // If the roughness value of an material is below this treshold we fallback to perfectly specular event.
    // If we didn't do this, we would end up with an extremely high values of sampling PDF.
    static constexpr Float SpecularEventRoughnessTreshold = 0.005f;

    virtual ~BSDF() = default;

    struct SamplingContext
    {
        // inputs
        const Material& material;
        SampledMaterialParameters materialParam;
        const math::Vector4 outgoingDir;
        Wavelength& wavelength; // non-const, because can trigger dispersion
        math::Random& randomGenerator;

        // outputs
        Color outColor = Color::Zero();
        math::Vector4 outIncomingDir = math::Vector4::Zero();
        Float outPdf = 0.0f;
        EventType outEventType = NullEvent;
    };

    struct EvaluationContext
    {
        const Material& material;
        SampledMaterialParameters materialParam;
        const Wavelength& wavelength;
        const math::Vector4 outgoingDir;
        const math::Vector4 incomingDir;
    };

    // get debug name
    virtual const char* GetName() const = 0;

    // Importance sample the BSDF
    // Generates incoming light direction for given outgoing ray direction.
    // Returns ray weight (NdotL multiplied) as well as sampling probability of the generated direction.
    virtual bool Sample(SamplingContext& ctx) const = 0;

    // Evaluate BSDF
    // Optionally returns probability of sampling this direction
    // NOTE: the result is NdotL multiplied
    virtual const Color Evaluate(const EvaluationContext& ctx, Float* outDirectPdfW = nullptr) const = 0;
};

} // namespace rt
