cmake_minimum_required(VERSION 3.11)
project(DataVisualizer VERSION 1.0.0 LANGUAGES CXX)
set(RAYLIB
${PROJECT_SOURCE_DIR}/include/raylib/lib/raylib.dll
${PROJECT_SOURCE_DIR}/include/raylib/lib/libraylibdll.a)

include_directories(include/raylib/include)
include_directories(include/SLL)
include_directories(include/Heap)
include_directories(include/AVL)
include_directories(include/General)
include_directories(include/Graph)
add_executable(${PROJECT_NAME} 
src/General/button.cpp
src/General/GlobalVar.cpp
src/General/tinyfiledialogs.cpp
src/General/TextBox.cpp
src/SLL/SLL_display.cpp
src/SLL/LinkedListAnimation.cpp
src/SLL/LinkedList.cpp
src/Heap/Heap_display.cpp
src/Heap/Heap.cpp
src/Heap/HeapAnimation.cpp
src/Graph/Dijkstra.cpp
src/Graph/Draw_GraphMenu.cpp
src/Graph/Graph_Display.cpp
src/Graph/Graph_Menu.cpp
src/Graph/graph.cpp
src/Graph/MST_Kruskal.cpp
src/main.cpp
)

file(COPY ${RAYLIB} DESTINATION ${PROJECT_SOURCE_DIR}/build)
target_link_libraries(${PROJECT_NAME} PUBLIC ${RAYLIB})