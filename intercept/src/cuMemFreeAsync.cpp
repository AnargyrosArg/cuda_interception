#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemFreeAsync)(
CUdeviceptr, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemFreeAsync(CUdeviceptr dptr, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemFreeAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemFreeAsync)
		{
			//fetch the original function addr using dlsym
			original_cuMemFreeAsync = (CUresult (*)(
			CUdeviceptr, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemFreeAsync");
			fprintf(stderr, "original_cuMemFreeAsync:%p\n", original_cuMemFreeAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemFreeAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemFreeAsync(
		dptr, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}