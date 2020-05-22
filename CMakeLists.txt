cmake_minimum_required(VERSION 3.16)
project(Projet_v1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Projet_v1 main.cpp Bucket.hpp Car.hpp CarFlow.hpp Carrier.hpp
        Course.hpp CrossingLightManagement.hpp Crossroads.hpp Global.hpp
        Timer.hpp TrafficLight.hpp Woodcutter.hpp Worker.hpp)