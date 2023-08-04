#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEventSynchronize)(
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEventSynchronize(CUevent hEvent) {
		fprintf(stderr, "===============\ncuEventSynchronize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEventSynchronize)
		{
			//fetch the original function addr using dlsym
			original_cuEventSynchronize = (CUresult (*)(
			CUevent)
			) dlsym(original_libcuda_handle, "cuEventSynchronize");
			fprintf(stderr, "original_cuEventSynchronize:%p\n", original_cuEventSynchronize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEventSynchronize():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuEventSynchronize(
		hEvent
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}