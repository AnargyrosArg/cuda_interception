#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemGetAccess)(
unsigned long long *, 
const CUmemLocation *, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemGetAccess(unsigned long long *flags, const CUmemLocation *location, CUdeviceptr ptr) {
		fprintf(stderr, "===============\ncuMemGetAccess()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemGetAccess)
		{
			//fetch the original function addr using dlsym
			original_cuMemGetAccess = (CUresult (*)(
			unsigned long long *, 
			const CUmemLocation *, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuMemGetAccess");
			fprintf(stderr, "original_cuMemGetAccess:%p\n", original_cuMemGetAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemGetAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemGetAccess(
		flags, 
		location, 
		ptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}