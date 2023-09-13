#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaImportExternalMemory)(
cudaExternalMemory_t *, 
const struct cudaExternalMemoryHandleDesc *
);
extern "C"
{
	cudaError_t cudaImportExternalMemory(cudaExternalMemory_t *extMem_out, const struct cudaExternalMemoryHandleDesc *memHandleDesc) {
		fprintf(stderr, "===============\ncudaImportExternalMemory()\n");
		char* __dlerror;
		if (!original_cudaImportExternalMemory)
		{
			//fetch the original function addr using dlsym
			original_cudaImportExternalMemory = (cudaError_t (*)(
			cudaExternalMemory_t *, 
			const struct cudaExternalMemoryHandleDesc *)
			) dlsym(RTLD_NEXT, "cudaImportExternalMemory");
			fprintf(stderr, "original_cudaImportExternalMemory:%p\n", original_cudaImportExternalMemory);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaImportExternalMemory():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaImportExternalMemory(
		extMem_out, 
		memHandleDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}