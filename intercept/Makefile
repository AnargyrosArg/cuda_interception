SRC = ./src
BUILD_DIR = ./build

SOURCE_FILES = $(wildcard $(SRC)/*.cpp)
TMP_SOURCE_FILES = $(SOURCE_FILES:$(SRC)/%=%)
OBJ_FILES = $(addprefix $(BUILD_DIR)/,$(TMP_SOURCE_FILES:.cpp=.o))

CWD = $(shell pwd)

.PHONY: src

interceptlib.so: $(OBJ_FILES) #-Wl,-soname,libcuda.so.515.43.04
	g++ $(OBJ_FILES) -o libintercept.so -fPIC -shared -D_GNU_SOURCE -Wl,-rpath,'$(CWD)' -Wl,-rpath,'/home1/public/argyrosan/.local/lib/python3.9/site-packages/torch/lib/'


#Compiles each source file into its object file individually 
$(BUILD_DIR)/%.o : $(SRC)/%.cpp $(BUILD_DIR)
	g++ -O3 -I${CUDA_HOME}/include -fPIC -shared -D_GNU_SOURCE  -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)/*.o *.so

#Rule to make sure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
