#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecEventWaitNodeSetEvent)(
CUgraphExec, 
CUgraphNode, 
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecEventWaitNodeSetEvent(CUgraphExec hGraphExec, CUgraphNode hNode, CUevent event) {
		fprintf(stderr, "===============\ncuGraphExecEventWaitNodeSetEvent()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecEventWaitNodeSetEvent)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecEventWaitNodeSetEvent = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			CUevent)
			) dlsym(original_libcuda_handle, "cuGraphExecEventWaitNodeSetEvent");
			fprintf(stderr, "original_cuGraphExecEventWaitNodeSetEvent:%p\n", original_cuGraphExecEventWaitNodeSetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecEventWaitNodeSetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphExecEventWaitNodeSetEvent(
		hGraphExec, 
		hNode, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}