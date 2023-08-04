#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolDestroy)(
CUmemoryPool
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolDestroy(CUmemoryPool pool) {
		fprintf(stderr, "===============\ncuMemPoolDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolDestroy = (CUresult (*)(
			CUmemoryPool)
			) dlsym(original_libcuda_handle, "cuMemPoolDestroy");
			fprintf(stderr, "original_cuMemPoolDestroy:%p\n", original_cuMemPoolDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemPoolDestroy(
		pool
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}