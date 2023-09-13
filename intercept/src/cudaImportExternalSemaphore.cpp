#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaImportExternalSemaphore)(
cudaExternalSemaphore_t *, 
const struct cudaExternalSemaphoreHandleDesc *
);
extern "C"
{
	cudaError_t cudaImportExternalSemaphore(cudaExternalSemaphore_t *extSem_out, const struct cudaExternalSemaphoreHandleDesc *semHandleDesc) {
		fprintf(stderr, "===============\ncudaImportExternalSemaphore()\n");
		char* __dlerror;
		if (!original_cudaImportExternalSemaphore)
		{
			//fetch the original function addr using dlsym
			original_cudaImportExternalSemaphore = (cudaError_t (*)(
			cudaExternalSemaphore_t *, 
			const struct cudaExternalSemaphoreHandleDesc *)
			) dlsym(RTLD_NEXT, "cudaImportExternalSemaphore");
			fprintf(stderr, "original_cudaImportExternalSemaphore:%p\n", original_cudaImportExternalSemaphore);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaImportExternalSemaphore():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaImportExternalSemaphore(
		extSem_out, 
		semHandleDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}