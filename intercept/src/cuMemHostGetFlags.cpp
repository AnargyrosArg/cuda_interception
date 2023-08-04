#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemHostGetFlags)(
unsigned int *, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemHostGetFlags(unsigned int *pFlags, void *p) {
		fprintf(stderr, "===============\ncuMemHostGetFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemHostGetFlags)
		{
			//fetch the original function addr using dlsym
			original_cuMemHostGetFlags = (CUresult (*)(
			unsigned int *, 
			void *)
			) dlsym(original_libcuda_handle, "cuMemHostGetFlags");
			fprintf(stderr, "original_cuMemHostGetFlags:%p\n", original_cuMemHostGetFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemHostGetFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemHostGetFlags(
		pFlags, 
		p
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}