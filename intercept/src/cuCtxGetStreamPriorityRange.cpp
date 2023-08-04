#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetStreamPriorityRange)(
int *, 
int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetStreamPriorityRange(int *leastPriority, int *greatestPriority) {
		fprintf(stderr, "===============\ncuCtxGetStreamPriorityRange()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetStreamPriorityRange)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetStreamPriorityRange = (CUresult (*)(
			int *, 
			int *)
			) dlsym(original_libcuda_handle, "cuCtxGetStreamPriorityRange");
			fprintf(stderr, "original_cuCtxGetStreamPriorityRange:%p\n", original_cuCtxGetStreamPriorityRange);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetStreamPriorityRange():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuCtxGetStreamPriorityRange(
		leastPriority, 
		greatestPriority
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}