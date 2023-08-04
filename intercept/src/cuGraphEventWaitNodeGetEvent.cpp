#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphEventWaitNodeGetEvent)(
CUgraphNode, 
CUevent *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphEventWaitNodeGetEvent(CUgraphNode hNode, CUevent *event_out) {
		fprintf(stderr, "===============\ncuGraphEventWaitNodeGetEvent()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphEventWaitNodeGetEvent)
		{
			//fetch the original function addr using dlsym
			original_cuGraphEventWaitNodeGetEvent = (CUresult (*)(
			CUgraphNode, 
			CUevent *)
			) dlsym(original_libcuda_handle, "cuGraphEventWaitNodeGetEvent");
			fprintf(stderr, "original_cuGraphEventWaitNodeGetEvent:%p\n", original_cuGraphEventWaitNodeGetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphEventWaitNodeGetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphEventWaitNodeGetEvent(
		hNode, 
		event_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}