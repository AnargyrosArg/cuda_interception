#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetChannelDesc)(
struct cudaChannelFormatDesc *, 
cudaArray_const_t
);
extern "C"
{
	cudaError_t cudaGetChannelDesc(struct cudaChannelFormatDesc *desc, cudaArray_const_t array) {
		fprintf(stderr, "===============\ncudaGetChannelDesc()\n");
		char* __dlerror;
		if (!original_cudaGetChannelDesc)
		{
			//fetch the original function addr using dlsym
			original_cudaGetChannelDesc = (cudaError_t (*)(
			struct cudaChannelFormatDesc *, 
			cudaArray_const_t)
			) dlsym(RTLD_NEXT, "cudaGetChannelDesc");
			fprintf(stderr, "original_cudaGetChannelDesc:%p\n", original_cudaGetChannelDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetChannelDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetChannelDesc(
		desc, 
		array
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}