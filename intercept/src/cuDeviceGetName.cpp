#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetName)(
char *, 
int, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetName(char *name, int len, CUdevice dev) {
		fprintf(stderr, "===============\ncuDeviceGetName()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetName)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetName = (CUresult (*)(
			char *, 
			int, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetName");
			fprintf(stderr, "original_cuDeviceGetName:%p\n", original_cuDeviceGetName);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetName():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGetName(
		name, 
		len, 
		dev
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}