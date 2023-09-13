#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolSetAccess)(
cudaMemPool_t, 
const struct cudaMemAccessDesc *, 
size_t
);
extern "C"
{
	cudaError_t cudaMemPoolSetAccess(cudaMemPool_t memPool, const struct cudaMemAccessDesc *descList, size_t count) {
		fprintf(stderr, "===============\ncudaMemPoolSetAccess()\n");
		char* __dlerror;
		if (!original_cudaMemPoolSetAccess)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolSetAccess = (cudaError_t (*)(
			cudaMemPool_t, 
			const struct cudaMemAccessDesc *, 
			size_t)
			) dlsym(RTLD_NEXT, "cudaMemPoolSetAccess");
			fprintf(stderr, "original_cudaMemPoolSetAccess:%p\n", original_cudaMemPoolSetAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolSetAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolSetAccess(
		memPool, 
		descList, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}