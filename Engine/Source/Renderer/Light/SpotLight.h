#pragma once

#include "Light.h"

namespace Core
{
    class CE_API SpotLight : public Light
    {
    private:
        static CeU32 globalID;
        CeU32 id;
        float cutOff;
        float outerCutOff;

        Vector3 specular;
        float intensity;
        float constant;
        float linear;
        float quadratic;

    public:
        SpotLight();
        ~SpotLight();

        // Inline getters and setters
        inline CeU32 GetID() const { return id; }
        inline void SetID(CeU32 newID) { id = newID; }

        inline float GetIntensity() const { return intensity; }
        inline void SetIntensity(float newIntensity) { intensity = newIntensity; }

        inline float GetCutOff() const { return cutOff; }
        inline void SetCutOff(float newCutOff) { cutOff = newCutOff; }

        inline float GetOuterCutOff() const { return outerCutOff; }
        inline void SetOuterCutOff(float newOuterCutOff) { outerCutOff = newOuterCutOff; }

        inline Vector3 *GetSpecular() { return &specular; }

        inline float GetConstant() const { return constant; }
        inline void SetConstant(float newConstant) { constant = newConstant; }

        inline float GetLinear() const { return linear; }
        inline void SetLinear(float newLinear) { linear = newLinear; }

        inline float GetQuadratic() const { return quadratic; }
        inline void SetQuadratic(float newQuadratic) { quadratic = newQuadratic; }

        void Update();

        static void IncrementGlobalID();
        static void SetGlobalIDZero();
        static void DecrementGlobalID();
        static CeU32 GetSpotLightGID();
    };
} // namespace Core
