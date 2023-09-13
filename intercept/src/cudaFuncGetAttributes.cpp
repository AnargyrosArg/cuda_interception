#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFuncGetAttributes)(
struct cudaFuncAttributes *, 
const void *
);
extern "C"
{
	cudaError_t cudaFuncGetAttributes(struct cudaFuncAttributes *attr, const void *func) {
		fprintf(stderr, "===============\ncudaFuncGetAttributes()\n");
		char* __dlerror;
		if (!original_cudaFuncGetAttributes)
		{
			//fetch the original function addr using dlsym
			original_cudaFuncGetAttributes = (cudaError_t (*)(
			struct cudaFuncAttributes *, 
			const void *)
			) dlsym(RTLD_NEXT, "cudaFuncGetAttributes");
			fprintf(stderr, "original_cudaFuncGetAttributes:%p\n", original_cudaFuncGetAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFuncGetAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFuncGetAttributes(
		attr, 
		func
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}