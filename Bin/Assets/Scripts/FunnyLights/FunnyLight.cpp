#include "FunnyLight.h"

void FunnyLight::OnStart() {}

void FunnyLight::OnUpdate()
{
    auto p = GetComponent<Core::PointLightComponent>();

    auto color = p->light->GetColor();

    if (color->r > 255)
        color->r = 0;

    if (color->g > 255)
        color->g = 0;

    if (color->b > 255)
        color->b = 0;

    color->r += 1;
    color->g += 1;
    color->b += 1;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(FunnyLight);
