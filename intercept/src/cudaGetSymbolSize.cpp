#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetSymbolSize)(
size_t *, 
const void *
);
extern "C"
{
	cudaError_t cudaGetSymbolSize(size_t *size, const void *symbol) {
		fprintf(stderr, "===============\ncudaGetSymbolSize()\n");
		char* __dlerror;
		if (!original_cudaGetSymbolSize)
		{
			//fetch the original function addr using dlsym
			original_cudaGetSymbolSize = (cudaError_t (*)(
			size_t *, 
			const void *)
			) dlsym(RTLD_NEXT, "cudaGetSymbolSize");
			fprintf(stderr, "original_cudaGetSymbolSize:%p\n", original_cudaGetSymbolSize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetSymbolSize():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetSymbolSize(
		size, 
		symbol
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}