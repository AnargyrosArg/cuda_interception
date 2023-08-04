#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEventRecordWithFlags)(
CUevent, 
CUstream, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEventRecordWithFlags(CUevent hEvent, CUstream hStream, unsigned int flags) {
		fprintf(stderr, "===============\ncuEventRecordWithFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEventRecordWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cuEventRecordWithFlags = (CUresult (*)(
			CUevent, 
			CUstream, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuEventRecordWithFlags");
			fprintf(stderr, "original_cuEventRecordWithFlags:%p\n", original_cuEventRecordWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEventRecordWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuEventRecordWithFlags(
		hEvent, 
		hStream, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}