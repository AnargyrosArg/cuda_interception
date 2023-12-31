#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamEndCapture)(
CUstream, 
CUgraph *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamEndCapture(CUstream hStream, CUgraph *phGraph) {
		fprintf(stderr, "===============\ncuStreamEndCapture()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamEndCapture)
		{
			//fetch the original function addr using dlsym
			original_cuStreamEndCapture = (CUresult (*)(
			CUstream, 
			CUgraph *)
			) dlsym(original_libcuda_handle, "cuStreamEndCapture");
			fprintf(stderr, "original_cuStreamEndCapture:%p\n", original_cuStreamEndCapture);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamEndCapture():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamEndCapture(
		hStream, 
		phGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}