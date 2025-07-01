cmake_minimum_required(VERSION 3.16)
project(SpaceInvadersQT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(RESOURCES Resources.qrc)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(
        QT_NO_DEBUG_OUTPUT
        QT_NO_INFO_OUTPUT
        QT_NO_WARNING_OUTPUT
        QT_NO_CRITICAL_OUTPUT
    )
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Concurrent)

file(GLOB_RECURSE GAME_LEVEL_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Levels/*.yaml"
    "${CMAKE_CURRENT_SOURCE_DIR}/Blueprints/*.yaml"
)

set(HEADERS
    Config/GameContext.h
    Game/Audio/SoundBuffer.h
    Game/Audio/SoundDevice.h
    Game/Audio/SoundInfo.h
    Game/Audio/SoundManager.h
    Game/Audio/SoundSource.h
    Game/CollisionDetection/BruteForce.h
    Game/CollisionDetection/BVHNode.h
    Game/CollisionDetection/BVHTree.h
    Game/CollisionDetection/CollisionDetector.h
    Game/CollisionDetection/Quadtree.h
    Game/CollisionDetection/ThreadedCollisionDetector.h
    Game/Core/GameHUD.h
    Game/Core/GameRunnerView.h
    Game/Core/GameState.h
    Game/Core/LevelSelectorView.h
    Game/Core/MainMenuView.h
    Game/Core/MenuTextItem.h
    Game/Core/MenuView.h
    Game/Core/PauseMenuView.h
    Game/Core/BenchmarkPromptView.h
    Game/Core/TogglableMenuTextItem.h
    Game/Levels/Enemy.h
    Game/Levels/Formation.h
    Game/Levels/Level.h
    Game/Levels/LevelLoader.h
    Game/Levels/LevelManager.h
    Game/Levels/SpawnEvent.h
    Game/Movement/AxisMovementStrategy.h
    Game/Movement/MovementStrategy.h
    Game/Movement/MovementStrategyLoader.h
    GameObjects/AttractableGameObject.h
    GameObjects/Collectables/Collectable.h
    GameObjects/Collectables/Health.h
    GameObjects/Collectables/Stellar.h
    GameObjects/Collectables/StellarPool.h
    GameObjects/GameObject.h
    GameObjects/GameObjectBlueprint.h
    GameObjects/GameObjectBuilder.h
    GameObjects/GameObjectLoader.h
    GameObjects/PrototypeRegistry.h
    GameObjects/Projectiles/EnemyProjectile.h
    GameObjects/Projectiles/PlayerProjectile.h
    GameObjects/Projectiles/Projectile.h
    GameObjects/Projectiles/Vortex.h
    GameObjects/Projectiles/WaveOfDestruction.h
    GameObjects/Ships/EnemyShip.h
    GameObjects/Ships/PlayerShip.h
    GameObjects/Ships/Ship.h
    GameObjects/Ships/ShipWithHealthBar.h
    Graphics/Animations/AnimatedItem.h
    Graphics/Animations/AnimationPlayer.h
    Graphics/CooldownItem.h
    Graphics/Effects/Effect.h
    Graphics/Effects/EffectManager.h
    Graphics/Effects/Particle.h
    Graphics/Effects/ParticleSystem.h
    Graphics/EnergyBar.h
    Graphics/HealthBar.h
    Graphics/PixmapRegistry.h
    Graphics/PixmapLibrary.h
    Graphics/ProgressBar.h
    Graphics/TextureRegistry.h
    UI/FPSCounter.h
    UI/GameObjectCounter.h
    UI/MainWindow.h
    Utils/Math/MathConstants.h
    Utils/Math/MathFunctions.h
    Utils/BoundsChecker.h
    Utils/CustomMessageHandler.h
    Utils/PerformanceBenchmark.h
    Utils/Utils.h
    Weapons/PrimaryWeapon.h
    Weapons/SecondaryWeapon.h
    Weapons/Weapon.h
    Weapons/WeaponBuilder.h
)

set(SOURCES
    Game/Audio/SoundBuffer.cpp
    Game/Audio/SoundDevice.cpp
    Game/Audio/SoundManager.cpp
    Game/Audio/SoundSource.cpp
    Game/CollisionDetection/BVHNode.cpp
    Game/CollisionDetection/BVHTree.cpp
    Game/CollisionDetection/CollisionDetector.cpp
    Game/Core/GameHUD.cpp
    Game/Core/GameRunnerView.cpp
    Game/Core/GameState.cpp
    Game/Core/LevelSelectorView.cpp
    Game/Core/MainMenuView.cpp
    Game/Core/MenuTextItem.cpp
    Game/Core/MenuView.cpp
    Game/Core/PauseMenuView.cpp
    Game/Core/BenchmarkPromptView.cpp
    Game/Core/TogglableMenuTextItem.cpp
    Game/Levels/Enemy.cpp
    Game/Levels/Formation.cpp
    Game/Levels/Level.cpp
    Game/Levels/LevelLoader.cpp
    Game/Levels/LevelManager.cpp
    Game/Levels/SpawnEvent.cpp
    Game/Movement/MovementStrategyLoader.cpp
    GameObjects/AttractableGameObject.cpp
    GameObjects/Collectables/Collectable.cpp
    GameObjects/Collectables/Health.cpp
    GameObjects/Collectables/Stellar.cpp
    GameObjects/Collectables/StellarPool.cpp
    GameObjects/GameObject.cpp
    GameObjects/GameObjectBuilder.cpp
    GameObjects/GameObjectLoader.cpp
    GameObjects/Projectiles/EnemyProjectile.cpp
    GameObjects/Projectiles/PlayerProjectile.cpp
    GameObjects/Projectiles/Projectile.cpp
    GameObjects/Projectiles/Vortex.cpp
    GameObjects/Projectiles/WaveOfDestruction.cpp
    GameObjects/Ships/EnemyShip.cpp
    GameObjects/Ships/PlayerShip.cpp
    GameObjects/Ships/Ship.cpp
    GameObjects/Ships/ShipWithHealthBar.cpp
    Graphics/Effects/EffectManager.cpp
    Graphics/Effects/ParticleSystem.cpp
    Graphics/EnergyBar.cpp
    Graphics/HealthBar.cpp
    Graphics/ProgressBar.cpp
    Main/main.cpp
    UI/FPSCounter.cpp
    UI/GameObjectCounter.cpp
    UI/MainWindow.cpp
    Utils/BoundsChecker.cpp
    Utils/PerformanceBenchmark.cpp
    Utils/Utils.cpp
    Weapons/PrimaryWeapon.cpp
    Weapons/SecondaryWeapon.cpp
    Weapons/Weapon.cpp
    Weapons/WeaponBuilder.cpp
    Tests/test_collisions.cpp
)

set(UIS
    UI/MainWindow.ui
)

qt6_add_resources(RES_SpaceInvadersQT ${RESOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS} ${UIS} ${RESOURCES})

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RES_SpaceInvadersQT}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGLWidgets
        Qt6::Concurrent
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        YAML_CPP_STATIC_DEFINE
)

target_sources(${PROJECT_NAME}
  PRIVATE
    ${GAME_LEVEL_FILES}
)

option(PERFORMANCE_BENCHMARK "Build with performance benchmark" OFF)
if(PERFORMANCE_BENCHMARK)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PERFORMANCE_BENCHMARK)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Try vcpkg or system yaml-cpp first
find_package(yaml-cpp CONFIG QUIET)
if(NOT yaml-cpp_FOUND)
    message(STATUS "Using local Thirdparty yaml-cpp")
    target_include_directories(${PROJECT_NAME} PRIVATE Thirdparty/yaml-cpp/include)
    target_link_directories(${PROJECT_NAME} PRIVATE Thirdparty/yaml-cpp/build)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

# Thirdparty OpenAL & libsndfile (MinGW)
if(WIN32 AND MINGW)
    # Use bundled libs on Windows/Mingw
    target_include_directories(${PROJECT_NAME} PRIVATE Thirdparty/OpenAL/include)
    target_link_directories(${PROJECT_NAME} PRIVATE Thirdparty/OpenAL/libs/Win64)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL32)

    target_include_directories(${PROJECT_NAME} PRIVATE Thirdparty/libsndfile-1.2.2/include)
    target_link_directories(${PROJECT_NAME} PRIVATE Thirdparty/libsndfile-1.2.2/build)
    target_link_libraries(${PROJECT_NAME} PRIVATE sndfile)
elseif(WIN32)
    # Use system packages on Windows/MSVC (e.g. via vcpkg or Chocolatey)
    find_package(OpenAL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)
    find_package(SndFile REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE SndFile::sndfile)
else()
    # Use system packages everywhere else
    find_package(OpenAL REQUIRED)
    if(TARGET OpenAL::OpenAL)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)
    else()
        # Fall back to legacy target if imported target doesn't exist
        target_link_libraries(${PROJECT_NAME} PRIVATE openal)
    endif()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SNDFILE REQUIRED sndfile)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SNDFILE_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${SNDFILE_INCLUDE_DIRS})
endif()

if(QNX)
    set(INSTALL_PATH /tmp/${PROJECT_NAME}/bin)
elseif(UNIX AND NOT ANDROID)
    set(INSTALL_PATH /opt/${PROJECT_NAME}/bin)
else()
    set(INSTALL_PATH ${CMAKE_INSTALL_BINDIR})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_PATH})

# Enable testing
enable_testing()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(UnitTests
        Tests/test_collisions.cpp
    )

    target_link_libraries(UnitTests PRIVATE gtest_main Qt6::Core)

    add_test(NAME SpaceInvadersQT_UnitTests COMMAND UnitTests)
endif()


