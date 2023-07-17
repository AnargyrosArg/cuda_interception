#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceTotalMem_v2)(
size_t *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceTotalMem_v2(size_t *bytes, CUdevice dev) {
		fprintf(stderr, "cuDeviceTotalMem_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceTotalMem_v2)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceTotalMem_v2 = (CUresult (*)(
			size_t *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceTotalMem_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceTotalMem_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceTotalMem_v2(
		bytes, 
		dev
		);
	}
}