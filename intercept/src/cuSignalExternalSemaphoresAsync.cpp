#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuSignalExternalSemaphoresAsync)(
const CUexternalSemaphore *, 
const CUDA_EXTERNAL_SEMAPHORE_SIGNAL_PARAMS *, 
unsigned int, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuSignalExternalSemaphoresAsync(const CUexternalSemaphore *extSemArray, const CUDA_EXTERNAL_SEMAPHORE_SIGNAL_PARAMS *paramsArray, unsigned int numExtSems, CUstream stream) {
		fprintf(stderr, "cuSignalExternalSemaphoresAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuSignalExternalSemaphoresAsync)
		{
			//fetch the original function addr using dlsym
			original_cuSignalExternalSemaphoresAsync = (CUresult (*)(
			const CUexternalSemaphore *, 
			const CUDA_EXTERNAL_SEMAPHORE_SIGNAL_PARAMS *, 
			unsigned int, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuSignalExternalSemaphoresAsync");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuSignalExternalSemaphoresAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuSignalExternalSemaphoresAsync(
		extSemArray, 
		paramsArray, 
		numExtSems, 
		stream
		);
	}
}