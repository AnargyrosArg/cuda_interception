#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsMapResources)(
unsigned int, 
CUgraphicsResource *, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsMapResources(unsigned int count, CUgraphicsResource *resources, CUstream hStream) {
		fprintf(stderr, "cuGraphicsMapResources()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsMapResources)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsMapResources = (CUresult (*)(
			unsigned int, 
			CUgraphicsResource *, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuGraphicsMapResources");
			fprintf(stderr, "original_cuGraphicsMapResources:%p\n", original_cuGraphicsMapResources);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsMapResources():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphicsMapResources(
		count, 
		resources, 
		hStream
		);
	}
}