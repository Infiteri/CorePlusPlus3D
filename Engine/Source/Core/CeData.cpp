#include "CeData.h"
#include "Core/Logger.h"

namespace Core
{
    namespace Data
    {
        void Set::ClearDataBasedOnCurrentType()
        {
            if (!ShouldClear)
            {
                CE_CORE_WARN("Set::ClearDataBasedOnCurrentType: Should not be called.");
                return;
            }

            ShouldClear = false;
#if 1
            switch (Type)
            {
            case DataVec2:
            {
                Vector2 *d = (Vector2 *)Data;
                delete d;
            }
            break;

            case DataVec3:
            {
                Vector3 *d = (Vector3 *)Data;
                delete d;
            }
            break;

            case DataVec4:
            {
                Vector4 *d = (Vector4 *)Data;
                delete d;
            }
            break;

            case DataColor:
            {
                Color *d = (Color *)Data;
                delete d;
            }
            break;

            case DataNone:
            default:
                ShouldClear = true;
                break;
            }
#else
            CeMemory::TracePrintSize("Data: ", sizeof(Data));
            CeMemory::Free(Data);
#endif
        }

        void Set::SetupDefaultValuesBaseOnCurrentType()
        {
            ShouldClear = true;
            switch (Type)
            {
            case DataVec2:
                Data = new Vector2(0, 0);
                break;

            case DataVec3:
                Data = new Vector3(0, 0, 0);
                break;

            case DataVec4:
                Data = new Vector4(0, 0, 0, 0);
                break;

            case DataColor:
                Data = new Color(255, 255, 255, 255);
                break;

            case DataNone:
            default:
                break;
            }
        }

        std::string Set::TypeToString()
        {
            switch (Type)
            {
            case Data::DataVec2:
                return "Vec2";
                break;

            case Data::DataVec3:
                return "Vec3";
                break;

            case Data::DataVec4:
                return "Vec4";
                break;

            case Data::DataColor:
                return "Color";
                break;

            case Data::DataNone:
            default:
                return "None";
                break;
            }

            return "None";
        }

        void Set::TypeFromString(const std::string &val)
        {
            if (val == "Vec2")
                Type = Data::DataVec2;
            else if (val == "Vec3")
                Type = Data::DataVec3;

            else if (val == "Vec4")
                Type = Data::DataVec4;

            else if (val == "Color")
                Type = Data::DataColor;

            Type = Data::DataNone;
        }

        DataType StringToDataType(const std::string &val)
        {
            if (val == "Vec2")
                return Data::DataVec2;
            else if (val == "Vec3")
                return Data::DataVec3;

            else if (val == "Vec4")
                return Data::DataVec4;

            else if (val == "Color")
                return Data::DataColor;

            return Data::DataNone;
        }

    }
}