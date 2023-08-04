#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetAttribute)(
int *, 
CUdevice_attribute, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetAttribute(int *pi, CUdevice_attribute attrib, CUdevice dev) {
		fprintf(stderr, "===============\ncuDeviceGetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetAttribute = (CUresult (*)(
			int *, 
			CUdevice_attribute, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetAttribute");
			fprintf(stderr, "original_cuDeviceGetAttribute:%p\n", original_cuDeviceGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGetAttribute(
		pi, 
		attrib, 
		dev
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}