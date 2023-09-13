#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolExportPointer)(
struct cudaMemPoolPtrExportData *, 
void *
);
extern "C"
{
	cudaError_t cudaMemPoolExportPointer(struct cudaMemPoolPtrExportData *exportData, void *ptr) {
		fprintf(stderr, "===============\ncudaMemPoolExportPointer()\n");
		char* __dlerror;
		if (!original_cudaMemPoolExportPointer)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolExportPointer = (cudaError_t (*)(
			struct cudaMemPoolPtrExportData *, 
			void *)
			) dlsym(RTLD_NEXT, "cudaMemPoolExportPointer");
			fprintf(stderr, "original_cudaMemPoolExportPointer:%p\n", original_cudaMemPoolExportPointer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolExportPointer():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolExportPointer(
		exportData, 
		ptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}