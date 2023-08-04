#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphicsResourceSetMapFlags_v2)(
CUgraphicsResource, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphicsResourceSetMapFlags_v2(CUgraphicsResource resource, unsigned int flags) {
		fprintf(stderr, "===============\ncuGraphicsResourceSetMapFlags_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphicsResourceSetMapFlags_v2)
		{
			//fetch the original function addr using dlsym
			original_cuGraphicsResourceSetMapFlags_v2 = (CUresult (*)(
			CUgraphicsResource, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphicsResourceSetMapFlags_v2");
			fprintf(stderr, "original_cuGraphicsResourceSetMapFlags_v2:%p\n", original_cuGraphicsResourceSetMapFlags_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphicsResourceSetMapFlags_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphicsResourceSetMapFlags_v2(
		resource, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}