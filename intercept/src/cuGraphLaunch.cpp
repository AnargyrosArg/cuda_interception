#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphLaunch)(
CUgraphExec, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphLaunch(CUgraphExec hGraphExec, CUstream hStream) {
		fprintf(stderr, "cuGraphLaunch()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphLaunch)
		{
			//fetch the original function addr using dlsym
			original_cuGraphLaunch = (CUresult (*)(
			CUgraphExec, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuGraphLaunch");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphLaunch():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphLaunch(
		hGraphExec, 
		hStream
		);
	}
}