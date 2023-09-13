#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaArrayGetInfo)(
struct cudaChannelFormatDesc *, 
struct cudaExtent *, 
unsigned int *, 
cudaArray_t
);
extern "C"
{
	cudaError_t cudaArrayGetInfo(struct cudaChannelFormatDesc *desc, struct cudaExtent *extent, unsigned int *flags, cudaArray_t array) {
		fprintf(stderr, "===============\ncudaArrayGetInfo()\n");
		char* __dlerror;
		if (!original_cudaArrayGetInfo)
		{
			//fetch the original function addr using dlsym
			original_cudaArrayGetInfo = (cudaError_t (*)(
			struct cudaChannelFormatDesc *, 
			struct cudaExtent *, 
			unsigned int *, 
			cudaArray_t)
			) dlsym(RTLD_NEXT, "cudaArrayGetInfo");
			fprintf(stderr, "original_cudaArrayGetInfo:%p\n", original_cudaArrayGetInfo);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaArrayGetInfo():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaArrayGetInfo(
		desc, 
		extent, 
		flags, 
		array
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}