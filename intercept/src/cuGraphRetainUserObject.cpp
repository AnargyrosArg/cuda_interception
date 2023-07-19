#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphRetainUserObject)(
CUgraph, 
CUuserObject, 
unsigned int, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphRetainUserObject(CUgraph graph, CUuserObject object, unsigned int count, unsigned int flags) {
		fprintf(stderr, "cuGraphRetainUserObject()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphRetainUserObject)
		{
			//fetch the original function addr using dlsym
			original_cuGraphRetainUserObject = (CUresult (*)(
			CUgraph, 
			CUuserObject, 
			unsigned int, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphRetainUserObject");
			fprintf(stderr, "original_cuGraphRetainUserObject:%p\n", original_cuGraphRetainUserObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphRetainUserObject():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphRetainUserObject(
		graph, 
		object, 
		count, 
		flags
		);
	}
}