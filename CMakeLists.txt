cmake_minimum_required(VERSION 3.22)
project(network)
set(CMAKE_CXX_STANDARD 17)

# --------------------------------------------------------------------------------
# Prepare External dependencies
# --------------------------------------------------------------------------------
include(FetchContent)

# --------------------------------------------------------------------------------
# Flatbuffers
# --------------------------------------------------------------------------------
find_package(flatbuffers 23.5.26 EXACT CONFIG)
if (flatbuffers_FOUND)
    message("Using local installed flatbuffers")
else ()
    message("Flatbuffers not found locally building from source")
    SET(FLATBUFFERS_BUILD_TESTS OFF CACHE INTERNAL "")
    FetchContent_Declare(
            flatbuffers
            GIT_REPOSITORY https://github.com/google/flatbuffers.git
            GIT_TAG v23.5.26  # Specify the desired version or commit hash
    )
    FetchContent_MakeAvailable(flatbuffers)
    find_package(flatbuffers CONFIG REQUIRED)
endif ()

# --------------------------------------------------------------------------------
# YAML
# --------------------------------------------------------------------------------
find_package(yaml-cpp 0.8.0)
if (yaml-cpp_FOUND)
    message("Using local installed yaml-cpp")
else ()
    message("Yaml-cpp not found locally building from source")
    FetchContent_Declare(
            yaml-cpp
            GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
            GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
    )
    FetchContent_GetProperties(yaml-cpp)
    FetchContent_Populate(yaml-cpp)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif ()

# --------------------------------------------------------------------------------
# Prepare local project
# --------------------------------------------------------------------------------
file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE sources_msgs proto/*.cc)

add_executable(network main.cpp ${sources}
        main.cpp
)
target_include_directories(network PUBLIC include/)
target_link_libraries(network PUBLIC flatbuffers::flatbuffers yaml-cpp::yaml-cpp)
