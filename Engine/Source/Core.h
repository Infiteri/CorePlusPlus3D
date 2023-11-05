#include "Base.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Core/Instrumentation.h"
#include "Core/CeAssert.h"
#include "Core/Engine.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Layer/Layer.h"
#include "Core/Memory/CeMemory.h"

#include "Event/EventManager.h"
#include "Event/CeEvents.h"

#include "Platform/Platform.h"
#include "Platform/FileSystem.h"

#include "Project/Project.h"
#include "Project/ProjectSerializer.h"

#include "Math/Math.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"

#include "Renderer/Renderer.h"
#include "Renderer/Buffer/FrameBuffer.h"
#include "Renderer/Objects/Mesh.h"
#include "Renderer/Manager/MaterialManager.h"
#include "Renderer/Manager/TextureManager.h"
#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Camera/PerspectiveMovement.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/World.h"
#include "Scene/Components/Components.h"
#include "Scene/SceneSerializer.h"

#include "Script/ActorScript.h"
#include "Script/ScriptEngine.h"

#include "Utils/StringUtils.h"