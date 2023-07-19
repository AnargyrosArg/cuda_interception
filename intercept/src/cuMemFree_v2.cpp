#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemFree_v2)(
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemFree_v2(CUdeviceptr dptr) {
		fprintf(stderr, "cuMemFree_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemFree_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemFree_v2 = (CUresult (*)(
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuMemFree_v2");
			fprintf(stderr, "original_cuMemFree_v2:%p\n", original_cuMemFree_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemFree_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemFree_v2(
		dptr
		);
	}
}