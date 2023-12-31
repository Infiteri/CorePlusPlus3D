#pragma once

#include "Base.h"
#include <string>
#include "Math/Math.h"
#include "Math/Vectors.h"
#include "Renderer/Color.h"

namespace Core
{
    namespace Data
    {
        enum DataType
        {
            DataNone = 0,
            DataVec2,
            DataVec3,
            DataVec4,
            DataColor,
            DataFloat,
        };

        struct FloatContainer
        {
            float Value;

            FloatContainer(){};
            FloatContainer(float v) { Value = v; };
        };

        struct CE_API Set
        {
            std::string Name;
            DataType Type;
            void *Data;
            bool ShouldClear = false;

            Set();

            Set(Set *s)
            {
                From(s);
            };

            Set(CeU32 dataSize, void *_Data, DataType dataType, const std::string &_name)
            {
                ShouldClear = true;
                Type = dataType;
                Name = _name;

                switch (Type)
                {
                case DataVec2:
                {
                    Vector2 *other = static_cast<Vector2 *>(_Data);
                    Data = new Vector2(other->x, other->y);
                    break;
                }

                case DataVec3:
                {
                    Vector3 *other = static_cast<Vector3 *>(_Data);
                    Data = new Vector3(other->x, other->y, other->z);
                    break;
                }

                case DataVec4:
                {
                    Vector4 *other = static_cast<Vector4 *>(_Data);
                    Data = new Vector4(other->x, other->y, other->z, other->w);
                    break;
                }

                case DataColor:
                {
                    Color *other = static_cast<Color *>(_Data);
                    Data = new Color(other->r, other->g, other->b, other->a);
                    break;
                }

                case DataFloat:
                {
                    FloatContainer *other = static_cast<FloatContainer *>(_Data);
                    Data = new FloatContainer(other->Value);
                    break;
                }

                case DataNone:
                default:
                    break;
                }
            }

            void ClearDataBasedOnCurrentType();

            /// @brief ClearDataBasedOnCurrentType IS NOT CALLED! CALL AT YOUR OWN
            void SetupDefaultValuesBaseOnCurrentType();

            std::string TypeToString();
            void TypeFromString(const std::string &val);

            void From(Set *s);

            /// @brief Make sure that its the right type.
            /// @tparam T The type to cast to.
            /// @return The Data casted.
            template <typename T>
            T *As()
            {
                return (T *)Data;
            }
        };

        DataType StringToDataType(const std::string &val);
    }
}