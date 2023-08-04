#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphUpload)(
CUgraphExec, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphUpload(CUgraphExec hGraphExec, CUstream hStream) {
		fprintf(stderr, "===============\ncuGraphUpload()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphUpload)
		{
			//fetch the original function addr using dlsym
			original_cuGraphUpload = (CUresult (*)(
			CUgraphExec, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuGraphUpload");
			fprintf(stderr, "original_cuGraphUpload:%p\n", original_cuGraphUpload);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphUpload():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphUpload(
		hGraphExec, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}