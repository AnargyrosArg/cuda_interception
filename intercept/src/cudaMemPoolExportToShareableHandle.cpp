#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolExportToShareableHandle)(
void *, 
cudaMemPool_t, 
enum cudaMemAllocationHandleType, 
unsigned int
);
extern "C"
{
	cudaError_t cudaMemPoolExportToShareableHandle(void *shareableHandle, cudaMemPool_t memPool, enum cudaMemAllocationHandleType handleType, unsigned int flags) {
		fprintf(stderr, "===============\ncudaMemPoolExportToShareableHandle()\n");
		char* __dlerror;
		if (!original_cudaMemPoolExportToShareableHandle)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolExportToShareableHandle = (cudaError_t (*)(
			void *, 
			cudaMemPool_t, 
			enum cudaMemAllocationHandleType, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaMemPoolExportToShareableHandle");
			fprintf(stderr, "original_cudaMemPoolExportToShareableHandle:%p\n", original_cudaMemPoolExportToShareableHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolExportToShareableHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolExportToShareableHandle(
		shareableHandle, 
		memPool, 
		handleType, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}