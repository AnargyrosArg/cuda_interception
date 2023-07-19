#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEventRecord)(
CUevent, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEventRecord(CUevent hEvent, CUstream hStream) {
		fprintf(stderr, "cuEventRecord()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEventRecord)
		{
			//fetch the original function addr using dlsym
			original_cuEventRecord = (CUresult (*)(
			CUevent, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuEventRecord");
			fprintf(stderr, "original_cuEventRecord:%p\n", original_cuEventRecord);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEventRecord():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEventRecord(
		hEvent, 
		hStream
		);
	}
}