#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemRelease)(
CUmemGenericAllocationHandle
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemRelease(CUmemGenericAllocationHandle handle) {
		fprintf(stderr, "===============\ncuMemRelease()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemRelease)
		{
			//fetch the original function addr using dlsym
			original_cuMemRelease = (CUresult (*)(
			CUmemGenericAllocationHandle)
			) dlsym(original_libcuda_handle, "cuMemRelease");
			fprintf(stderr, "original_cuMemRelease:%p\n", original_cuMemRelease);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemRelease():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemRelease(
		handle
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}