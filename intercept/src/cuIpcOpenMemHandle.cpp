#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuIpcOpenMemHandle)(
CUdeviceptr *, 
CUipcMemHandle, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuIpcOpenMemHandle(CUdeviceptr *pdptr, CUipcMemHandle handle, unsigned int Flags) {
		fprintf(stderr, "cuIpcOpenMemHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuIpcOpenMemHandle)
		{
			//fetch the original function addr using dlsym
			original_cuIpcOpenMemHandle = (CUresult (*)(
			CUdeviceptr *, 
			CUipcMemHandle, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuIpcOpenMemHandle");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuIpcOpenMemHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuIpcOpenMemHandle(
		pdptr, 
		handle, 
		Flags
		);
	}
}