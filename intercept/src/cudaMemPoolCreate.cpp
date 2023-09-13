#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPoolCreate)(
cudaMemPool_t *, 
const struct cudaMemPoolProps *
);
extern "C"
{
	cudaError_t cudaMemPoolCreate(cudaMemPool_t *memPool, const struct cudaMemPoolProps *poolProps) {
		fprintf(stderr, "===============\ncudaMemPoolCreate()\n");
		char* __dlerror;
		if (!original_cudaMemPoolCreate)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPoolCreate = (cudaError_t (*)(
			cudaMemPool_t *, 
			const struct cudaMemPoolProps *)
			) dlsym(RTLD_NEXT, "cudaMemPoolCreate");
			fprintf(stderr, "original_cudaMemPoolCreate:%p\n", original_cudaMemPoolCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPoolCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPoolCreate(
		memPool, 
		poolProps
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}