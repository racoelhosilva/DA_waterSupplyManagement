cmake_minimum_required(VERSION 3.26)
project(DA_waterSupplyManagement)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(DA_waterSupplyManagement main.cpp
        src/Graph.cpp
        include/Graph.h
        src/Vertex.cpp
        include/Vertex.h
        src/Edge.cpp
        include/Edge.h
        src/WaterSupplyNetwork.cpp
        include/WaterSupplyNetwork.h
        src/Pipe.cpp
        include/Pipe.h
        src/ServicePoint.cpp
        include/ServicePoint.h
        src/Reservoir.cpp
        include/Reservoir.h
        src/PumpingStation.cpp
        include/PumpingStation.h
        src/DeliverySite.cpp
        include/DeliverySite.h)

add_subdirectory(docs)
