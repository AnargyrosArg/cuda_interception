#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuIpcGetEventHandle)(
CUipcEventHandle *, 
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuIpcGetEventHandle(CUipcEventHandle *pHandle, CUevent event) {
		fprintf(stderr, "===============\ncuIpcGetEventHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuIpcGetEventHandle)
		{
			//fetch the original function addr using dlsym
			original_cuIpcGetEventHandle = (CUresult (*)(
			CUipcEventHandle *, 
			CUevent)
			) dlsym(original_libcuda_handle, "cuIpcGetEventHandle");
			fprintf(stderr, "original_cuIpcGetEventHandle:%p\n", original_cuIpcGetEventHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuIpcGetEventHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuIpcGetEventHandle(
		pHandle, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}