#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamGetPriority)(
CUstream, 
int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamGetPriority(CUstream hStream, int *priority) {
		fprintf(stderr, "===============\ncuStreamGetPriority()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamGetPriority)
		{
			//fetch the original function addr using dlsym
			original_cuStreamGetPriority = (CUresult (*)(
			CUstream, 
			int *)
			) dlsym(original_libcuda_handle, "cuStreamGetPriority");
			fprintf(stderr, "original_cuStreamGetPriority:%p\n", original_cuStreamGetPriority);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamGetPriority():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamGetPriority(
		hStream, 
		priority
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}