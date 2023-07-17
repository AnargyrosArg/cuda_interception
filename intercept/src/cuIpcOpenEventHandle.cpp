#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuIpcOpenEventHandle)(
CUevent *, 
CUipcEventHandle
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuIpcOpenEventHandle(CUevent *phEvent, CUipcEventHandle handle) {
		fprintf(stderr, "cuIpcOpenEventHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuIpcOpenEventHandle)
		{
			//fetch the original function addr using dlsym
			original_cuIpcOpenEventHandle = (CUresult (*)(
			CUevent *, 
			CUipcEventHandle)
			) dlsym(original_libcuda_handle, "cuIpcOpenEventHandle");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuIpcOpenEventHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuIpcOpenEventHandle(
		phEvent, 
		handle
		);
	}
}