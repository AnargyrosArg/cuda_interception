#include <cudaProfiler.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuProfilerStop)(
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuProfilerStop() {
		fprintf(stderr, "cuProfilerStop()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuProfilerStop)
		{
			//fetch the original function addr using dlsym
			original_cuProfilerStop = (CUresult (*)(
			void)
			) dlsym(original_libcuda_handle, "cuProfilerStop");
			fprintf(stderr, "original_cuProfilerStop:%p\n", original_cuProfilerStop);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuProfilerStop():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuProfilerStop(
		);
	}
}