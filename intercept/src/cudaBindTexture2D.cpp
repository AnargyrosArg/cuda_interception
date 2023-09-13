#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaBindTexture2D)(
size_t *, 
const struct textureReference *, 
const void *, 
const struct cudaChannelFormatDesc *, 
size_t, 
size_t, 
size_t
);
extern "C"
{
	cudaError_t cudaBindTexture2D(size_t *offset, const struct textureReference *texref, const void *devPtr, const struct cudaChannelFormatDesc *desc, size_t width, size_t height, size_t pitch) {
		fprintf(stderr, "===============\ncudaBindTexture2D()\n");
		char* __dlerror;
		if (!original_cudaBindTexture2D)
		{
			//fetch the original function addr using dlsym
			original_cudaBindTexture2D = (cudaError_t (*)(
			size_t *, 
			const struct textureReference *, 
			const void *, 
			const struct cudaChannelFormatDesc *, 
			size_t, 
			size_t, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaBindTexture2D");
			fprintf(stderr, "original_cudaBindTexture2D:%p\n", original_cudaBindTexture2D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaBindTexture2D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaBindTexture2D(
		offset, 
		texref, 
		devPtr, 
		desc, 
		width, 
		height, 
		pitch
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}