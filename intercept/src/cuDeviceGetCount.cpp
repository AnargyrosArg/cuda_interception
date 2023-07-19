#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetCount)(
int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetCount(int *count) {
		fprintf(stderr, "cuDeviceGetCount()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetCount)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetCount = (CUresult (*)(
			int *)
			) dlsym(original_libcuda_handle, "cuDeviceGetCount");
			fprintf(stderr, "original_cuDeviceGetCount:%p\n", original_cuDeviceGetCount);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetCount():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetCount(
		count
		);
	}
}