#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolGetAccess)(
CUmemAccess_flags *, 
CUmemoryPool, 
CUmemLocation *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolGetAccess(CUmemAccess_flags *flags, CUmemoryPool memPool, CUmemLocation *location) {
		fprintf(stderr, "cuMemPoolGetAccess()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolGetAccess)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolGetAccess = (CUresult (*)(
			CUmemAccess_flags *, 
			CUmemoryPool, 
			CUmemLocation *)
			) dlsym(original_libcuda_handle, "cuMemPoolGetAccess");
			fprintf(stderr, "original_cuMemPoolGetAccess:%p\n", original_cuMemPoolGetAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolGetAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemPoolGetAccess(
		flags, 
		memPool, 
		location
		);
	}
}