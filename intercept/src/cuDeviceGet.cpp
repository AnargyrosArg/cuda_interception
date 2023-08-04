#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGet)(
CUdevice *, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGet(CUdevice *device, int ordinal) {
		fprintf(stderr, "===============\ncuDeviceGet()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGet)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGet = (CUresult (*)(
			CUdevice *, 
			int)
			) dlsym(original_libcuda_handle, "cuDeviceGet");
			fprintf(stderr, "original_cuDeviceGet:%p\n", original_cuDeviceGet);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGet():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGet(
		device, 
		ordinal
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}