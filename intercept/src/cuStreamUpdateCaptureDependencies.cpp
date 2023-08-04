#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamUpdateCaptureDependencies)(
CUstream, 
CUgraphNode *, 
size_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamUpdateCaptureDependencies(CUstream hStream, CUgraphNode *dependencies, size_t numDependencies, unsigned int flags) {
		fprintf(stderr, "===============\ncuStreamUpdateCaptureDependencies()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamUpdateCaptureDependencies)
		{
			//fetch the original function addr using dlsym
			original_cuStreamUpdateCaptureDependencies = (CUresult (*)(
			CUstream, 
			CUgraphNode *, 
			size_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamUpdateCaptureDependencies");
			fprintf(stderr, "original_cuStreamUpdateCaptureDependencies:%p\n", original_cuStreamUpdateCaptureDependencies);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamUpdateCaptureDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamUpdateCaptureDependencies(
		hStream, 
		dependencies, 
		numDependencies, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}