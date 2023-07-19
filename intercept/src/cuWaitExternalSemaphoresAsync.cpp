#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuWaitExternalSemaphoresAsync)(
const CUexternalSemaphore *, 
const CUDA_EXTERNAL_SEMAPHORE_WAIT_PARAMS *, 
unsigned int, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuWaitExternalSemaphoresAsync(const CUexternalSemaphore *extSemArray, const CUDA_EXTERNAL_SEMAPHORE_WAIT_PARAMS *paramsArray, unsigned int numExtSems, CUstream stream) {
		fprintf(stderr, "cuWaitExternalSemaphoresAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuWaitExternalSemaphoresAsync)
		{
			//fetch the original function addr using dlsym
			original_cuWaitExternalSemaphoresAsync = (CUresult (*)(
			const CUexternalSemaphore *, 
			const CUDA_EXTERNAL_SEMAPHORE_WAIT_PARAMS *, 
			unsigned int, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuWaitExternalSemaphoresAsync");
			fprintf(stderr, "original_cuWaitExternalSemaphoresAsync:%p\n", original_cuWaitExternalSemaphoresAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuWaitExternalSemaphoresAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuWaitExternalSemaphoresAsync(
		extSemArray, 
		paramsArray, 
		numExtSems, 
		stream
		);
	}
}