#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDriverGetVersion)(
int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDriverGetVersion(int *driverVersion) {
		fprintf(stderr, "===============\ncuDriverGetVersion()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDriverGetVersion)
		{
			//fetch the original function addr using dlsym
			original_cuDriverGetVersion = (CUresult (*)(
			int *)
			) dlsym(original_libcuda_handle, "cuDriverGetVersion");
			fprintf(stderr, "original_cuDriverGetVersion:%p\n", original_cuDriverGetVersion);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDriverGetVersion():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDriverGetVersion(
		driverVersion
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}