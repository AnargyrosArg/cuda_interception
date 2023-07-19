#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolTrimTo)(
CUmemoryPool, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolTrimTo(CUmemoryPool pool, size_t minBytesToKeep) {
		fprintf(stderr, "cuMemPoolTrimTo()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolTrimTo)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolTrimTo = (CUresult (*)(
			CUmemoryPool, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemPoolTrimTo");
			fprintf(stderr, "original_cuMemPoolTrimTo:%p\n", original_cuMemPoolTrimTo);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolTrimTo():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemPoolTrimTo(
		pool, 
		minBytesToKeep
		);
	}
}