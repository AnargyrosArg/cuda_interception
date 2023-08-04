#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetGraphMemAttribute)(
CUdevice, 
CUgraphMem_attribute, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetGraphMemAttribute(CUdevice device, CUgraphMem_attribute attr, void *value) {
		fprintf(stderr, "===============\ncuDeviceGetGraphMemAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetGraphMemAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetGraphMemAttribute = (CUresult (*)(
			CUdevice, 
			CUgraphMem_attribute, 
			void *)
			) dlsym(original_libcuda_handle, "cuDeviceGetGraphMemAttribute");
			fprintf(stderr, "original_cuDeviceGetGraphMemAttribute:%p\n", original_cuDeviceGetGraphMemAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetGraphMemAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGetGraphMemAttribute(
		device, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}