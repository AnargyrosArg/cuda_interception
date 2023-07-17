#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEventElapsedTime)(
float *, 
CUevent, 
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEventElapsedTime(float *pMilliseconds, CUevent hStart, CUevent hEnd) {
		fprintf(stderr, "cuEventElapsedTime()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEventElapsedTime)
		{
			//fetch the original function addr using dlsym
			original_cuEventElapsedTime = (CUresult (*)(
			float *, 
			CUevent, 
			CUevent)
			) dlsym(original_libcuda_handle, "cuEventElapsedTime");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEventElapsedTime():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEventElapsedTime(
		pMilliseconds, 
		hStart, 
		hEnd
		);
	}
}