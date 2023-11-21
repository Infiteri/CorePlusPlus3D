#include "FunnyLight.h"

void FunnyLight::OnStart() {}

void FunnyLight::OnUpdate()
{
    auto p = GetComponent<Core::PointLightComponent>();

    auto color = p->light->GetColor();

    float max = 5.0f;

    // if (color->r > 255)
    //     color->r = 0;
    // else
    //     color->r += Core::Math::RandomFloat(0, max);

    // if (color->g > 255)
    //     color->g = 0;
    // else
    //     color->g += Core::Math::RandomFloat(0, max);

    // if (color->b > 255)
    //     color->b = 0;
    // else
    //     color->b += Core::Math::RandomFloat(0, max);

    color->r = Core::Math::Lerp(0.0f, 1.0f, color->r / 1.0f) * 1;
    color->g = Core::Math::Lerp(0.0f, 1.0f, color->g / 1.0f) * 1;
    color->b = Core::Math::Lerp(0.0f, 1.0f, color->b / 1.0f) * 1;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(FunnyLight);
