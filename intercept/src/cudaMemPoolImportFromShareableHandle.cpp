#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolImportFromShareableHandle)(
cudaMemPool_t *, 
void *, 
enum cudaMemAllocationHandleType, 
unsigned int
);
extern "C"
{
	cudaError_t cudaMemPoolImportFromShareableHandle(cudaMemPool_t *memPool, void *shareableHandle, enum cudaMemAllocationHandleType handleType, unsigned int flags) {
		fprintf(stderr, "===============\ncudaMemPoolImportFromShareableHandle()\n");
		char* __dlerror;
		if (!original_cudaMemPoolImportFromShareableHandle)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolImportFromShareableHandle = (cudaError_t (*)(
			cudaMemPool_t *, 
			void *, 
			enum cudaMemAllocationHandleType, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaMemPoolImportFromShareableHandle");
			fprintf(stderr, "original_cudaMemPoolImportFromShareableHandle:%p\n", original_cudaMemPoolImportFromShareableHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolImportFromShareableHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolImportFromShareableHandle(
		memPool, 
		shareableHandle, 
		handleType, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}