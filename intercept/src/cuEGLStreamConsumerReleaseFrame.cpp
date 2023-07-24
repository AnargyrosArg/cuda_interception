#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEGLStreamConsumerReleaseFrame)(
CUeglStreamConnection *, 
CUgraphicsResource, 
CUstream *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEGLStreamConsumerReleaseFrame(CUeglStreamConnection *conn, CUgraphicsResource pCudaResource, CUstream *pStream) {
		fprintf(stderr, "cuEGLStreamConsumerReleaseFrame()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEGLStreamConsumerReleaseFrame)
		{
			//fetch the original function addr using dlsym
			original_cuEGLStreamConsumerReleaseFrame = (CUresult (*)(
			CUeglStreamConnection *, 
			CUgraphicsResource, 
			CUstream *)
			) dlsym(original_libcuda_handle, "cuEGLStreamConsumerReleaseFrame");
			fprintf(stderr, "original_cuEGLStreamConsumerReleaseFrame:%p\n", original_cuEGLStreamConsumerReleaseFrame);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEGLStreamConsumerReleaseFrame():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEGLStreamConsumerReleaseFrame(
		conn, 
		pCudaResource, 
		pStream
		);
	}
}