#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLaunchHostFunc)(
CUstream, 
CUhostFn, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLaunchHostFunc(CUstream hStream, CUhostFn fn, void *userData) {
		fprintf(stderr, "cuLaunchHostFunc()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLaunchHostFunc)
		{
			//fetch the original function addr using dlsym
			original_cuLaunchHostFunc = (CUresult (*)(
			CUstream, 
			CUhostFn, 
			void *)
			) dlsym(original_libcuda_handle, "cuLaunchHostFunc");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLaunchHostFunc():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLaunchHostFunc(
		hStream, 
		fn, 
		userData
		);
	}
}