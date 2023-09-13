#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMallocPitch)(
void **, 
size_t *, 
size_t, 
size_t
);
extern "C"
{
	cudaError_t cudaMallocPitch(void **devPtr, size_t *pitch, size_t width, size_t height) {
		fprintf(stderr, "===============\ncudaMallocPitch()\n");
		char* __dlerror;
		if (!original_cudaMallocPitch)
		{
			//fetch the original function addr using dlsym
			original_cudaMallocPitch = (cudaError_t (*)(
			void **, 
			size_t *, 
			size_t, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMallocPitch");
			fprintf(stderr, "original_cudaMallocPitch:%p\n", original_cudaMallocPitch);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMallocPitch():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMallocPitch(
		devPtr, 
		pitch, 
		width, 
		height
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}