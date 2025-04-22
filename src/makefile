# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = 	-std=c++20 \
			-I/opt/homebrew/include \
			-I../include/General \
			-I../include/Graph \
			-I../include/AVL \
			-I../include/SLL \
			-I../include/Heap \
			-I../include/Trie \

# Linker flags
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio

# Source file
SRC = $(wildcard *.cpp General/*.cpp Graph/*.cpp Heap/*.cpp SLL/*.cpp Trie/*.cpp)

# SRC = 	*.cpp \
		General/*.cpp \
		Graph/*.cpp \
		Heap/*.cpp \
		SLL/*.cpp

# Output ./executable
OUT = run

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS); ./$(OUT); rm -f $(OUT)

# Run and delete after execution
compile: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS);

# Clean target
clean:
	rm -f $(OUT)
