cmake_minimum_required(VERSION 3.30)
project(OBJViewer_)

set(CMAKE_CXX_STANDARD 20)

add_executable(OBJViewer_ main.cpp
        model/obj/OBJModel.h
        model/math/Vector2D.h
        model/math/Vector3D.h
        model/math/VecMathCommon.h
        model/obj/Material.h
        model/obj/MTLParser.h
        render/Renderer.h
        render/WinAPIRenderer.h
        model/loaders/ILoader.h
        model/loaders/OBJLoader.h
        model/Model.h
        controller/EventType.h
        controller/Event.h
        controller/IEventHandler.h
        controller/Controller.h
        controller/Transformer.h
        controller/Triangle.h)

if(WIN32)
    target_link_libraries(OBJViewer_ PRIVATE gdi32.lib gdiplus.lib)
    target_compile_definitions(OBJViewer_ PRIVATE UNICODE _UNICODE)
endif()