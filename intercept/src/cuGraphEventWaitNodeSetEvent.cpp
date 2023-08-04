#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphEventWaitNodeSetEvent)(
CUgraphNode, 
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphEventWaitNodeSetEvent(CUgraphNode hNode, CUevent event) {
		fprintf(stderr, "===============\ncuGraphEventWaitNodeSetEvent()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphEventWaitNodeSetEvent)
		{
			//fetch the original function addr using dlsym
			original_cuGraphEventWaitNodeSetEvent = (CUresult (*)(
			CUgraphNode, 
			CUevent)
			) dlsym(original_libcuda_handle, "cuGraphEventWaitNodeSetEvent");
			fprintf(stderr, "original_cuGraphEventWaitNodeSetEvent:%p\n", original_cuGraphEventWaitNodeSetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphEventWaitNodeSetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphEventWaitNodeSetEvent(
		hNode, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}