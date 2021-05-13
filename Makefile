include .env
CFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)\Include -I$(GLFW_PATH)\include -I$(GLM_PATH)
LDFLAGS = -L$(GLFW_PATH)\lib-mingw-w64 -L$(VULKAN_SDK_PATH)/Lib -lglfw3dll -lvulkan-1

# target = a.out - Compiles all .cpp files in src into a.out
a.exe: src/*.cpp
	g++ $(CFLAGS) -o a.exe src/*.cpp $(LDFLAGS)

.PHONY: test clean

test: a.exe
	./a.exe
clean:
	rm -Force a.out
	rm -Force *.spv

