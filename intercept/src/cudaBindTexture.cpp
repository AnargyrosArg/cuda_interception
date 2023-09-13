#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaBindTexture)(
size_t *, 
const struct textureReference *, 
const void *, 
const struct cudaChannelFormatDesc *, 
size_t
);
extern "C"
{
	cudaError_t cudaBindTexture(size_t *offset, const struct textureReference *texref, const void *devPtr, const struct cudaChannelFormatDesc *desc, size_t size) {
		fprintf(stderr, "===============\ncudaBindTexture()\n");
		char* __dlerror;
		if (!original_cudaBindTexture)
		{
			//fetch the original function addr using dlsym
			original_cudaBindTexture = (cudaError_t (*)(
			size_t *, 
			const struct textureReference *, 
			const void *, 
			const struct cudaChannelFormatDesc *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaBindTexture");
			fprintf(stderr, "original_cudaBindTexture:%p\n", original_cudaBindTexture);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaBindTexture():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaBindTexture(
		offset, 
		texref, 
		devPtr, 
		desc, 
		size
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}