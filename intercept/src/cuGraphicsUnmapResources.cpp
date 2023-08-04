#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsUnmapResources)(
unsigned int, 
CUgraphicsResource *, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsUnmapResources(unsigned int count, CUgraphicsResource *resources, CUstream hStream) {
		fprintf(stderr, "===============\ncuGraphicsUnmapResources()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsUnmapResources)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsUnmapResources = (CUresult (*)(
			unsigned int, 
			CUgraphicsResource *, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuGraphicsUnmapResources");
			fprintf(stderr, "original_cuGraphicsUnmapResources:%p\n", original_cuGraphicsUnmapResources);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsUnmapResources():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphicsUnmapResources(
		count, 
		resources, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}