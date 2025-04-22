cmake_minimum_required(VERSION 3.11)
project(DataVisualizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) 
set(RAYLIB
${PROJECT_SOURCE_DIR}/include/raylib/lib/libraylibdll.a)


# Include path cho raylib
include_directories(include/raylib/include)


# Include path cho các phần khác
include_directories(
    include/General
    include/SLL
    include/Heap
    include/Graph
    include/Trie
)

# Khai báo các file nguồn
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
    src/General/AnnouncementBox.cpp
    src/Graph/Dijkstra.cpp
    src/Graph/Draw_GraphMenu.cpp
    src/Graph/Graph_Display.cpp
    src/Graph/Graph_Menu.cpp
    src/Graph/graph.cpp
    src/Graph/MST_Kruskal.cpp
    src/Trie/Trie.cpp
    src/Trie/Trie_display.cpp
)

# Đường dẫn thư viện raylib
set(RAYLIB_DLL "${PROJECT_SOURCE_DIR}/include/raylib/lib/raylib.dll")
set(RAYLIB_LIB "${PROJECT_SOURCE_DIR}/include/raylib/lib/libraylibdll.a")

# Copy DLL để chạy được sau khi build
file(COPY ${RAYLIB_DLL} DESTINATION ${PROJECT_BINARY_DIR})

# Liên kết với thư viện raylib
target_link_libraries(${PROJECT_NAME} PUBLIC ${RAYLIB_LIB})

