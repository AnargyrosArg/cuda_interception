#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaExternalMemoryGetMappedBuffer)(
void **, 
cudaExternalMemory_t, 
const struct cudaExternalMemoryBufferDesc *
);
extern "C"
{
	cudaError_t cudaExternalMemoryGetMappedBuffer(void **devPtr, cudaExternalMemory_t extMem, const struct cudaExternalMemoryBufferDesc *bufferDesc) {
		fprintf(stderr, "===============\ncudaExternalMemoryGetMappedBuffer()\n");
		char* __dlerror;
		if (!original_cudaExternalMemoryGetMappedBuffer)
		{
			//fetch the original function addr using dlsym
			original_cudaExternalMemoryGetMappedBuffer = (cudaError_t (*)(
			void **, 
			cudaExternalMemory_t, 
			const struct cudaExternalMemoryBufferDesc *)
			) dlsym(RTLD_NEXT, "cudaExternalMemoryGetMappedBuffer");
			fprintf(stderr, "original_cudaExternalMemoryGetMappedBuffer:%p\n", original_cudaExternalMemoryGetMappedBuffer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaExternalMemoryGetMappedBuffer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaExternalMemoryGetMappedBuffer(
		devPtr, 
		extMem, 
		bufferDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}