#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceSetGraphMemAttribute)(
CUdevice, 
CUgraphMem_attribute, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceSetGraphMemAttribute(CUdevice device, CUgraphMem_attribute attr, void *value) {
		fprintf(stderr, "cuDeviceSetGraphMemAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceSetGraphMemAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceSetGraphMemAttribute = (CUresult (*)(
			CUdevice, 
			CUgraphMem_attribute, 
			void *)
			) dlsym(original_libcuda_handle, "cuDeviceSetGraphMemAttribute");
			fprintf(stderr, "original_cuDeviceSetGraphMemAttribute:%p\n", original_cuDeviceSetGraphMemAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceSetGraphMemAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceSetGraphMemAttribute(
		device, 
		attr, 
		value
		);
	}
}