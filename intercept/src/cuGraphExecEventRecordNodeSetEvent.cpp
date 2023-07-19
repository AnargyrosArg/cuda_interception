#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecEventRecordNodeSetEvent)(
CUgraphExec, 
CUgraphNode, 
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecEventRecordNodeSetEvent(CUgraphExec hGraphExec, CUgraphNode hNode, CUevent event) {
		fprintf(stderr, "cuGraphExecEventRecordNodeSetEvent()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecEventRecordNodeSetEvent)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecEventRecordNodeSetEvent = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			CUevent)
			) dlsym(original_libcuda_handle, "cuGraphExecEventRecordNodeSetEvent");
			fprintf(stderr, "original_cuGraphExecEventRecordNodeSetEvent:%p\n", original_cuGraphExecEventRecordNodeSetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecEventRecordNodeSetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExecEventRecordNodeSetEvent(
		hGraphExec, 
		hNode, 
		event
		);
	}
}