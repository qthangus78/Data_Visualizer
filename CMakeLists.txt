cmake_minimum_required(VERSION 3.11)
project(DataVisualizer VERSION 1.0.0 LANGUAGES CXX)
set(RAYLIB
${PROJECT_SOURCE_DIR}/include/raylib/lib/raylib.dll
${PROJECT_SOURCE_DIR}/include/raylib/lib/libraylibdll.a)

include_directories(include/raylib/include)
include_directories(include)
add_executable(${PROJECT_NAME} 
src/main.cpp
src/SLL_display.cpp
src/tinyfiledialogs.cpp
src/LinkedListAnimation.cpp
src/LinkedList.cpp
src/GlobalVar.cpp
src/button.cpp
)

file(COPY ${RAYLIB} DESTINATION ${PROJECT_SOURCE_DIR}/build)
target_link_libraries(${PROJECT_NAME} PUBLIC ${RAYLIB})