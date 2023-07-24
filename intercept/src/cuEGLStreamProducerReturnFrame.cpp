#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEGLStreamProducerReturnFrame)(
CUeglStreamConnection *, 
CUeglFrame *, 
CUstream *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEGLStreamProducerReturnFrame(CUeglStreamConnection *conn, CUeglFrame *eglframe, CUstream *pStream) {
		fprintf(stderr, "cuEGLStreamProducerReturnFrame()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEGLStreamProducerReturnFrame)
		{
			//fetch the original function addr using dlsym
			original_cuEGLStreamProducerReturnFrame = (CUresult (*)(
			CUeglStreamConnection *, 
			CUeglFrame *, 
			CUstream *)
			) dlsym(original_libcuda_handle, "cuEGLStreamProducerReturnFrame");
			fprintf(stderr, "original_cuEGLStreamProducerReturnFrame:%p\n", original_cuEGLStreamProducerReturnFrame);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEGLStreamProducerReturnFrame():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEGLStreamProducerReturnFrame(
		conn, 
		eglframe, 
		pStream
		);
	}
}