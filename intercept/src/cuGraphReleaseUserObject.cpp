#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphReleaseUserObject)(
CUgraph, 
CUuserObject, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphReleaseUserObject(CUgraph graph, CUuserObject object, unsigned int count) {
		fprintf(stderr, "===============\ncuGraphReleaseUserObject()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphReleaseUserObject)
		{
			//fetch the original function addr using dlsym
			original_cuGraphReleaseUserObject = (CUresult (*)(
			CUgraph, 
			CUuserObject, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphReleaseUserObject");
			fprintf(stderr, "original_cuGraphReleaseUserObject:%p\n", original_cuGraphReleaseUserObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphReleaseUserObject():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphReleaseUserObject(
		graph, 
		object, 
		count
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}