#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuIpcCloseMemHandle)(
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuIpcCloseMemHandle(CUdeviceptr dptr) {
		fprintf(stderr, "cuIpcCloseMemHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuIpcCloseMemHandle)
		{
			//fetch the original function addr using dlsym
			original_cuIpcCloseMemHandle = (CUresult (*)(
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuIpcCloseMemHandle");
			fprintf(stderr, "original_cuIpcCloseMemHandle:%p\n", original_cuIpcCloseMemHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuIpcCloseMemHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuIpcCloseMemHandle(
		dptr
		);
	}
}