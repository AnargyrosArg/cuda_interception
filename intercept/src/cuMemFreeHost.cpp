#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemFreeHost)(
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemFreeHost(void *p) {
		fprintf(stderr, "===============\ncuMemFreeHost()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemFreeHost)
		{
			//fetch the original function addr using dlsym
			original_cuMemFreeHost = (CUresult (*)(
			void *)
			) dlsym(original_libcuda_handle, "cuMemFreeHost");
			fprintf(stderr, "original_cuMemFreeHost:%p\n", original_cuMemFreeHost);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemFreeHost():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemFreeHost(
		p
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}