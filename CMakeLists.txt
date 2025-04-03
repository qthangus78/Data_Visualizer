cmake_minimum_required(VERSION 3.11)
project(DataVisualizer VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17) 
set(RAYLIB
${PROJECT_SOURCE_DIR}/include/raylib/lib/libraylibdll.a)

include_directories(include/raylib/include)
include_directories(include/SLL)
include_directories(include/Heap)
include_directories(include)
include_directories(include/Graph)
include_directories(include/General)
include_directories(include/AVL)
include_directories(include/Trie)
add_executable(${PROJECT_NAME} 
src/SLL/SLL_display.cpp
src/SLL/LinkedListAnimation.cpp
src/SLL/LinkedList.cpp
src/Heap/Heap_display.cpp
src/Heap/Heap.cpp
src/Heap/HeapAnimation.cpp
src/main.cpp
src/General/GlobalVar.cpp
src/General/tinyfiledialogs.cpp
src/General/button.cpp
src/General/TextBox.cpp
src/Graph/graph.cpp
src/Graph/Graph_Display.cpp
src/Graph/Graph_Menu.cpp
src/Trie/Trie.cpp
src/Trie/Trie_display.cpp
)

file(COPY ${PROJECT_SOURCE_DIR}/include/raylib/lib/raylib.dll
     DESTINATION ${PROJECT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${RAYLIB})