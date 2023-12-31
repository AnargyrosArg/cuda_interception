#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsUnregisterResource)(
CUgraphicsResource
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsUnregisterResource(CUgraphicsResource resource) {
		fprintf(stderr, "===============\ncuGraphicsUnregisterResource()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsUnregisterResource)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsUnregisterResource = (CUresult (*)(
			CUgraphicsResource)
			) dlsym(original_libcuda_handle, "cuGraphicsUnregisterResource");
			fprintf(stderr, "original_cuGraphicsUnregisterResource:%p\n", original_cuGraphicsUnregisterResource);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsUnregisterResource():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphicsUnregisterResource(
		resource
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}