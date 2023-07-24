#include <cudaProfiler.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuProfilerInitialize)(
const char *, 
const char *, 
CUoutput_mode
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuProfilerInitialize(const char *configFile, const char *outputFile, CUoutput_mode outputMode) {
		fprintf(stderr, "cuProfilerInitialize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuProfilerInitialize)
		{
			//fetch the original function addr using dlsym
			original_cuProfilerInitialize = (CUresult (*)(
			const char *, 
			const char *, 
			CUoutput_mode)
			) dlsym(original_libcuda_handle, "cuProfilerInitialize");
			fprintf(stderr, "original_cuProfilerInitialize:%p\n", original_cuProfilerInitialize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuProfilerInitialize():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuProfilerInitialize(
		configFile, 
		outputFile, 
		outputMode
		);
	}
}