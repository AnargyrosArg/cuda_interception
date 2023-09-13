#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolGetAccess)(
enum cudaMemAccessFlags *, 
cudaMemPool_t, 
struct cudaMemLocation *
);
extern "C"
{
	cudaError_t cudaMemPoolGetAccess(enum cudaMemAccessFlags *flags, cudaMemPool_t memPool, struct cudaMemLocation *location) {
		fprintf(stderr, "===============\ncudaMemPoolGetAccess()\n");
		char* __dlerror;
		if (!original_cudaMemPoolGetAccess)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolGetAccess = (cudaError_t (*)(
			enum cudaMemAccessFlags *, 
			cudaMemPool_t, 
			struct cudaMemLocation *)
			) dlsym(RTLD_NEXT, "cudaMemPoolGetAccess");
			fprintf(stderr, "original_cudaMemPoolGetAccess:%p\n", original_cudaMemPoolGetAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolGetAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolGetAccess(
		flags, 
		memPool, 
		location
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}