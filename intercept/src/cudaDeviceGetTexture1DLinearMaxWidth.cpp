#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaDeviceGetTexture1DLinearMaxWidth)(
size_t *, 
const struct cudaChannelFormatDesc *, 
int
);
extern "C"
{
	cudaError_t cudaDeviceGetTexture1DLinearMaxWidth(size_t *maxWidthInElements, const struct cudaChannelFormatDesc *fmtDesc, int device) {
		fprintf(stderr, "===============\ncudaDeviceGetTexture1DLinearMaxWidth()\n");
		char* __dlerror;
		if (!original_cudaDeviceGetTexture1DLinearMaxWidth)
		{
			//fetch the original function addr using dlsym
			original_cudaDeviceGetTexture1DLinearMaxWidth = (cudaError_t (*)(
			size_t *, 
			const struct cudaChannelFormatDesc *, 
			int)
			) dlsym(RTLD_NEXT, "cudaDeviceGetTexture1DLinearMaxWidth");
			fprintf(stderr, "original_cudaDeviceGetTexture1DLinearMaxWidth:%p\n", original_cudaDeviceGetTexture1DLinearMaxWidth);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaDeviceGetTexture1DLinearMaxWidth():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaDeviceGetTexture1DLinearMaxWidth(
		maxWidthInElements, 
		fmtDesc, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}