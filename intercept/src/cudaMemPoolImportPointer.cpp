#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolImportPointer)(
void **, 
cudaMemPool_t, 
struct cudaMemPoolPtrExportData *
);
extern "C"
{
	cudaError_t cudaMemPoolImportPointer(void **ptr, cudaMemPool_t memPool, struct cudaMemPoolPtrExportData *exportData) {
		fprintf(stderr, "===============\ncudaMemPoolImportPointer()\n");
		char* __dlerror;
		if (!original_cudaMemPoolImportPointer)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolImportPointer = (cudaError_t (*)(
			void **, 
			cudaMemPool_t, 
			struct cudaMemPoolPtrExportData *)
			) dlsym(RTLD_NEXT, "cudaMemPoolImportPointer");
			fprintf(stderr, "original_cudaMemPoolImportPointer:%p\n", original_cudaMemPoolImportPointer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolImportPointer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolImportPointer(
		ptr, 
		memPool, 
		exportData
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}