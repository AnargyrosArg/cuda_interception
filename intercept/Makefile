SRC = ./src
BUILD_DIR = ./build

SOURCE_FILES = $(wildcard $(SRC)/*.cpp)
TMP_SOURCE_FILES = $(SOURCE_FILES:$(SRC)/%=%)
OBJ_FILES = $(addprefix $(BUILD_DIR)/,$(TMP_SOURCE_FILES:.cpp=.o))

CWD = $(shell pwd)

.PHONY: src

all: libintercept.so libcublas_static.so libcublasLt_static.so libcudnn_static.so


libcublas_static.so: 
	g++ $(CUDA_HOME)/lib64/libcublas_static.a -o libcublas_static.so -fPIC -shared -D_GNU_SOURCE -rdynamic -Wl,-rpath,'$(CWD)' 


libcublasLt_static.so: 
	g++ $(CUDA_HOME)/lib64/libcublasLt_static.a -o libcublasLt_static.so -fPIC -shared -D_GNU_SOURCE -rdynamic -Wl,-rpath,'$(CWD)'

libcudnn_static.so:
	g++ -o libcudnn_static.so -fPIC -shared -D_GNU_SOURCE -L$(CUDA_HOME)/lib64/libcudnn_ops_infer_static.a -L$(CUDA_HOME)/lib64/libcudnn_cnn_infer_static.a -L$(CUDA_HOME)/lib64/libcudnn_cnn_train_static.a  -Wl,-rpath,'$(CWD)'


libintercept.so: $(OBJ_FILES) $(BUILD_DIR)
	g++ $(OBJ_FILES) -o libintercept.so -fPIC -shared -D_GNU_SOURCE -Wl,-rpath,'$(CWD)' -Wl,-rpath,'/home1/public/argyrosan/.local/lib/python3.9/site-packages/torch/lib/'

#Compiles each source file into its object file individually 
$(BUILD_DIR)/%.o : $(SRC)/%.cpp 
	g++ -O3 -I${CUDA_HOME}/include -g -fPIC -shared -D_GNU_SOURCE  -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)/*.o *.so

#Rule to make sure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
