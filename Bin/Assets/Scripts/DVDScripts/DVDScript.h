#pragma once

#include <Core.h>

#include <vector>

using namespace Core;

class DVDScript : public Core::ActorScript
{
public:
    Vector2 velocity;

    // -- AABB -------------
    AABBComponent *aabb;
    AABBComponent *left;
    AABBComponent *right;
    AABBComponent *top;
    AABBComponent *bottom;
    // ---------------------

    DVDScript();
    ~DVDScript();

    void OnStart();
    void OnUpdate();
};

CE_DEFINE_ACTOR_SCRIPT(DVDScript);
