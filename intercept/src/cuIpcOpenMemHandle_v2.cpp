#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuIpcOpenMemHandle_v2)(
CUdeviceptr *, 
CUipcMemHandle, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuIpcOpenMemHandle_v2(CUdeviceptr *pdptr, CUipcMemHandle handle, unsigned int Flags) {
		fprintf(stderr, "cuIpcOpenMemHandle_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuIpcOpenMemHandle_v2)
		{
			//fetch the original function addr using dlsym
			original_cuIpcOpenMemHandle_v2 = (CUresult (*)(
			CUdeviceptr *, 
			CUipcMemHandle, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuIpcOpenMemHandle_v2");
			fprintf(stderr, "original_cuIpcOpenMemHandle_v2:%p\n", original_cuIpcOpenMemHandle_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuIpcOpenMemHandle_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuIpcOpenMemHandle_v2(
		pdptr, 
		handle, 
		Flags
		);
	}
}