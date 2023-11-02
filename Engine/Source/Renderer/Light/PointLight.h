#pragma once

#include "Light.h"

namespace Core
{
    class CE_API PointLight : public Light
    {
    private:
        Vector3 specular;
        CeU32 id;

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        float radius = 5;
        float intensity = 2;

    public:
        PointLight();
        ~PointLight();

        void Update();

        inline Vector3 *GetSpecular() { return &specular; };

        inline float GetConstant() { return constant; };
        inline void SetConstant(float v) { constant = v; };

        inline float GetLinear() { return linear; };
        inline void SetLinear(float v) { linear = v; };

        inline float GetQuadratic() { return quadratic; };
        inline void SetQuadratic(float v) { quadratic = v; };

        inline float GetIntensity() { return intensity; };
        inline void SetIntensity(float v) { intensity = v; };

        inline float GetRadius() { return radius; };
        inline void SetRadius(float v) { radius = v; };

        static void IncremenetGolbalID();
        static void SetGlobalID0();
        static void DecremenetGolbalID();
        static CeU32 GetPointLightGID();
    };

}