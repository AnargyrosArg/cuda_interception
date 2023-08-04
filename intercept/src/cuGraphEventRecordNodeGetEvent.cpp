#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphEventRecordNodeGetEvent)(
CUgraphNode, 
CUevent *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphEventRecordNodeGetEvent(CUgraphNode hNode, CUevent *event_out) {
		fprintf(stderr, "===============\ncuGraphEventRecordNodeGetEvent()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphEventRecordNodeGetEvent)
		{
			//fetch the original function addr using dlsym
			original_cuGraphEventRecordNodeGetEvent = (CUresult (*)(
			CUgraphNode, 
			CUevent *)
			) dlsym(original_libcuda_handle, "cuGraphEventRecordNodeGetEvent");
			fprintf(stderr, "original_cuGraphEventRecordNodeGetEvent:%p\n", original_cuGraphEventRecordNodeGetEvent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphEventRecordNodeGetEvent():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphEventRecordNodeGetEvent(
		hNode, 
		event_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}