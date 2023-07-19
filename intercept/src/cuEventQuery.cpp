#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEventQuery)(
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEventQuery(CUevent hEvent) {
		fprintf(stderr, "cuEventQuery()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEventQuery)
		{
			//fetch the original function addr using dlsym
			original_cuEventQuery = (CUresult (*)(
			CUevent)
			) dlsym(original_libcuda_handle, "cuEventQuery");
			fprintf(stderr, "original_cuEventQuery:%p\n", original_cuEventQuery);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEventQuery():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEventQuery(
		hEvent
		);
	}
}