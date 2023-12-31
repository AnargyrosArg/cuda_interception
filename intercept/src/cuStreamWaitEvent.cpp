#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamWaitEvent)(
CUstream, 
CUevent, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamWaitEvent(CUstream hStream, CUevent hEvent, unsigned int Flags) {
		fprintf(stderr, "===============\ncuStreamWaitEvent()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamWaitEvent)
		{
			//fetch the original function addr using dlsym
			original_cuStreamWaitEvent = (CUresult (*)(
			CUstream, 
			CUevent, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamWaitEvent");
			fprintf(stderr, "original_cuStreamWaitEvent:%p\n", original_cuStreamWaitEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamWaitEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamWaitEvent(
		hStream, 
		hEvent, 
		Flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}