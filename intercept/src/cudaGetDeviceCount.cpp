#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetDeviceCount)(
int *
);
extern "C"
{
	cudaError_t cudaGetDeviceCount(int *count) {
		fprintf(stderr, "===============\ncudaGetDeviceCount()\n");
		char* __dlerror;
		if (!original_cudaGetDeviceCount)
		{
			//fetch the original function addr using dlsym
			original_cudaGetDeviceCount = (cudaError_t (*)(
			int *)
			) dlsym(RTLD_NEXT, "cudaGetDeviceCount");
			fprintf(stderr, "original_cudaGetDeviceCount:%p\n", original_cudaGetDeviceCount);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetDeviceCount():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetDeviceCount(
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}