#pragma once

#include "Base.h"
#include "Math/Matrix4.h"

#include <string>

namespace Core
{
    class Shader
    {
    private:
        CeU32 id;

    public:
        Shader(const std::string &vertex, const std::string &fragment);
        ~Shader();

        void Destroy();

        void Use();
        void Int(int x, const char *name);
        void Mat4(float *x, const char *name);
        void Mat4(Matrix4 *m, const char *name);
        void Mat4(Matrix4 m, const char *name);

        void Vec3(float x, float y, float z, const char *name);
        void Vec3(Vector3 *m, const char *name);
        void Vec3(Vector3 m, const char *name);

        void Vec4(float x, float y, float z, float w, const char *name);
        void Vec4(Vector4 *m, const char *name);
        void Vec4(Vector4 m, const char *name);

        CeU32 GetID();
        CeU32 GetUniLoc(const char *name);
    };
}