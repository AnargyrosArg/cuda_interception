#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphInstantiate)(
CUgraphExec *, 
CUgraph, 
CUgraphNode *, 
char *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphInstantiate(CUgraphExec *phGraphExec, CUgraph hGraph, CUgraphNode *phErrorNode, char *logBuffer, size_t bufferSize) {
		fprintf(stderr, "cuGraphInstantiate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphInstantiate)
		{
			//fetch the original function addr using dlsym
			original_cuGraphInstantiate = (CUresult (*)(
			CUgraphExec *, 
			CUgraph, 
			CUgraphNode *, 
			char *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuGraphInstantiate");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphInstantiate():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphInstantiate(
		phGraphExec, 
		hGraph, 
		phErrorNode, 
		logBuffer, 
		bufferSize
		);
	}
}