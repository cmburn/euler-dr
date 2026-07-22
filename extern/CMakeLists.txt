set(SDL_STATIC ON)
set(PHYSFS_BUILD_TEST OFF)
set(BUILD_SHARED_LIBS OFF)
set(SLANG_ENABLE_SLANG_RHI OFF)
set(SLANG_ENABLE_TESTING OFF)
set(VK2D_BUILD_SDL OFF)

if (EULER_NATIVE_BUILD)
    set(MRUBY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include
            CACHE PATH "Path to the mruby include directory" FORCE)

    file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/mruby-headers.txt MRUBY_HEADERS)
    message(STATUS "MRuby headers: ${MRUBY_HEADERS}")

    add_library(mruby INTERFACE
            ${MRUBY_HEADERS}
    )

    set(MRUBY_LIBRARIES
            ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby.a
            ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/lib/libmruby_core.a
    )

    target_include_directories(mruby INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/mruby/build/host/include
    )

    target_compile_definitions(mruby INTERFACE
            MRB_USE_CXX_EXCEPTION
            MRB_USE_CXX_ABI
            MRB_USE_SET
            HAVE_MRUBY_IO_GEM
            MRB_USE_RATIONAL
            MRB_USE_COMPLEX
            MRB_USE_BIGINT
            MRB_USE_DEBUG_HOOK
    )

    target_link_libraries(mruby INTERFACE ${MRUBY_LIBRARIES})

    if (WIN32)
        target_link_libraries(mruby INTERFACE ws2_32 wsock32)
    endif ()
endif ()

add_subdirectory(eigen)

if (EULER_PHYSICS_BUILD)
    add_subdirectory(box2d)
endif ()

if (EULER_GRAPHICS_BUILD)
    add_library(nuklear INTERFACE
            Nuklear/nuklear.h
    )
    target_include_directories(nuklear INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/Nuklear
    )
endif ()

if (EULER_NET_BUILD)
    add_subdirectory(protobuf)
    add_subdirectory(GameNetworkingSockets)
endif ()

if (EULER_WORLD_BUILD)
    add_subdirectory(nlohmann-json)
endif ()

if (EULER_NATIVE_BUILD)
    add_subdirectory(SDL3)
    add_subdirectory(physfs)
    target_sources(physfs-static PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras/physfssdl3.c
            ${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras/physfssdl3.h
    )

    target_include_directories(physfs-static INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/physfs/extras>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SDL/include>
            $<INSTALL_INTERFACE:include/>
    )

    add_subdirectory(Vulkan2D)
    target_include_directories(Vulkan2D PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D
    )
endif ()