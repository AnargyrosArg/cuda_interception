#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuIpcGetMemHandle)(
CUipcMemHandle *, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuIpcGetMemHandle(CUipcMemHandle *pHandle, CUdeviceptr dptr) {
		fprintf(stderr, "cuIpcGetMemHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuIpcGetMemHandle)
		{
			//fetch the original function addr using dlsym
			original_cuIpcGetMemHandle = (CUresult (*)(
			CUipcMemHandle *, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuIpcGetMemHandle");
			fprintf(stderr, "original_cuIpcGetMemHandle:%p\n", original_cuIpcGetMemHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuIpcGetMemHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuIpcGetMemHandle(
		pHandle, 
		dptr
		);
	}
}