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
# Prepare local project
# --------------------------------------------------------------------------------
file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE sources_msgs proto/*.cc)

add_executable(network main.cpp ${sources}
        main.cpp
)
target_include_directories(network PUBLIC include/)
target_link_libraries(network PUBLIC flatbuffers::flatbuffers)
