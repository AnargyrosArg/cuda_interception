#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAlloc_v2)(
CUdeviceptr *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAlloc_v2(CUdeviceptr *dptr, size_t bytesize) {
		fprintf(stderr, "===============\ncuMemAlloc_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAlloc_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemAlloc_v2 = (CUresult (*)(
			CUdeviceptr *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemAlloc_v2");
			fprintf(stderr, "original_cuMemAlloc_v2:%p\n", original_cuMemAlloc_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAlloc_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemAlloc_v2(
		dptr, 
		bytesize
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}