#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFlushGPUDirectRDMAWrites)(
CUflushGPUDirectRDMAWritesTarget, 
CUflushGPUDirectRDMAWritesScope
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFlushGPUDirectRDMAWrites(CUflushGPUDirectRDMAWritesTarget target, CUflushGPUDirectRDMAWritesScope scope) {
		fprintf(stderr, "cuFlushGPUDirectRDMAWrites()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFlushGPUDirectRDMAWrites)
		{
			//fetch the original function addr using dlsym
			original_cuFlushGPUDirectRDMAWrites = (CUresult (*)(
			CUflushGPUDirectRDMAWritesTarget, 
			CUflushGPUDirectRDMAWritesScope)
			) dlsym(original_libcuda_handle, "cuFlushGPUDirectRDMAWrites");
			fprintf(stderr, "original_cuFlushGPUDirectRDMAWrites:%p\n", original_cuFlushGPUDirectRDMAWrites);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFlushGPUDirectRDMAWrites():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuFlushGPUDirectRDMAWrites(
		target, 
		scope
		);
	}
}