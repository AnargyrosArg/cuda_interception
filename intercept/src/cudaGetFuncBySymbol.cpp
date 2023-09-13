#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGetFuncBySymbol)(
cudaFunction_t *, 
const void *
);
extern "C"
{
	cudaError_t cudaGetFuncBySymbol(cudaFunction_t *functionPtr, const void *symbolPtr) {
		fprintf(stderr, "===============\ncudaGetFuncBySymbol()\n");
		char* __dlerror;
		if (!original_cudaGetFuncBySymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaGetFuncBySymbol = (cudaError_t (*)(
			cudaFunction_t *, 
			const void *)
			) dlsym(RTLD_NEXT, "cudaGetFuncBySymbol");
			fprintf(stderr, "original_cudaGetFuncBySymbol:%p\n", original_cudaGetFuncBySymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGetFuncBySymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGetFuncBySymbol(
		functionPtr, 
		symbolPtr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}