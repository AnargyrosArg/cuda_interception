#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetDriverEntryPoint)(
const char *, 
void **, 
unsigned long long
);
extern "C"
{
	cudaError_t cudaGetDriverEntryPoint(const char *symbol, void **funcPtr, unsigned long long flags) {
		fprintf(stderr, "===============\ncudaGetDriverEntryPoint()\n");
		char* __dlerror;
		if (!original_cudaGetDriverEntryPoint)
		{
			//fetch the original function addr using dlsym
			original_cudaGetDriverEntryPoint = (cudaError_t (*)(
			const char *, 
			void **, 
			unsigned long long)
			) dlsym(RTLD_NEXT, "cudaGetDriverEntryPoint");
			fprintf(stderr, "original_cudaGetDriverEntryPoint:%p\n", original_cudaGetDriverEntryPoint);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetDriverEntryPoint():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetDriverEntryPoint(
		symbol, 
		funcPtr, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}