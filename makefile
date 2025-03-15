# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++20 -I/opt/homebrew/include

# Linker flags
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio

# Source file
SRC = *.cpp

# Output ./executable
OUT = run

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

# Run and delete after execution
execute: $(OUT)
	./$(OUT) && rm -f $(OUT)

# Clean target
clean:
	rm -f $(OUT)
