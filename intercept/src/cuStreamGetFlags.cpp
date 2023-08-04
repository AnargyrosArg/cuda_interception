#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamGetFlags)(
CUstream, 
unsigned int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamGetFlags(CUstream hStream, unsigned int *flags) {
		fprintf(stderr, "===============\ncuStreamGetFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamGetFlags)
		{
			//fetch the original function addr using dlsym
			original_cuStreamGetFlags = (CUresult (*)(
			CUstream, 
			unsigned int *)
			) dlsym(original_libcuda_handle, "cuStreamGetFlags");
			fprintf(stderr, "original_cuStreamGetFlags:%p\n", original_cuStreamGetFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamGetFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamGetFlags(
		hStream, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}