#include <cudaProfiler.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuProfilerStart)(
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuProfilerStart() {
		fprintf(stderr, "cuProfilerStart()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuProfilerStart)
		{
			//fetch the original function addr using dlsym
			original_cuProfilerStart = (CUresult (*)(
			void)
			) dlsym(original_libcuda_handle, "cuProfilerStart");
			fprintf(stderr, "original_cuProfilerStart:%p\n", original_cuProfilerStart);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuProfilerStart():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuProfilerStart(
		);
	}
}