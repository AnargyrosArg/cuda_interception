#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuInit)(
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuInit(unsigned int Flags) {
		fprintf(stderr, "===============\ncuInit()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuInit)
		{
			//fetch the original function addr using dlsym
			original_cuInit = (CUresult (*)(
			unsigned int)
			) dlsym(original_libcuda_handle, "cuInit");
			fprintf(stderr, "original_cuInit:%p\n", original_cuInit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuInit():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuInit(
		Flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}