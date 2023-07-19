#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamGetCaptureInfo_v2)(
CUstream, 
CUstreamCaptureStatus *, 
cuuint64_t *, 
CUgraph *, 
const CUgraphNode **, 
size_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamGetCaptureInfo_v2(CUstream hStream, CUstreamCaptureStatus *captureStatus_out, cuuint64_t *id_out, CUgraph *graph_out, const CUgraphNode **dependencies_out, size_t *numDependencies_out) {
		fprintf(stderr, "cuStreamGetCaptureInfo_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamGetCaptureInfo_v2)
		{
			//fetch the original function addr using dlsym
			original_cuStreamGetCaptureInfo_v2 = (CUresult (*)(
			CUstream, 
			CUstreamCaptureStatus *, 
			cuuint64_t *, 
			CUgraph *, 
			const CUgraphNode **, 
			size_t *)
			) dlsym(original_libcuda_handle, "cuStreamGetCaptureInfo_v2");
			fprintf(stderr, "original_cuStreamGetCaptureInfo_v2:%p\n", original_cuStreamGetCaptureInfo_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamGetCaptureInfo_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamGetCaptureInfo_v2(
		hStream, 
		captureStatus_out, 
		id_out, 
		graph_out, 
		dependencies_out, 
		numDependencies_out
		);
	}
}