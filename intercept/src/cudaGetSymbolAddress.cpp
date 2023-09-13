#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetSymbolAddress)(
void **, 
const void *
);
extern "C"
{
	cudaError_t cudaGetSymbolAddress(void **devPtr, const void *symbol) {
		fprintf(stderr, "===============\ncudaGetSymbolAddress()\n");
		char* __dlerror;
		if (!original_cudaGetSymbolAddress)
		{
			//fetch the original function addr using dlsym
			original_cudaGetSymbolAddress = (cudaError_t (*)(
			void **, 
			const void *)
			) dlsym(RTLD_NEXT, "cudaGetSymbolAddress");
			fprintf(stderr, "original_cudaGetSymbolAddress:%p\n", original_cudaGetSymbolAddress);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetSymbolAddress():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetSymbolAddress(
		devPtr, 
		symbol
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}