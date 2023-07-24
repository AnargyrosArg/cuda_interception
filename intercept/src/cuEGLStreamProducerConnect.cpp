#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEGLStreamProducerConnect)(
CUeglStreamConnection *, 
EGLStreamKHR, 
EGLint, 
EGLint
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEGLStreamProducerConnect(CUeglStreamConnection *conn, EGLStreamKHR stream, EGLint width, EGLint height) {
		fprintf(stderr, "cuEGLStreamProducerConnect()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEGLStreamProducerConnect)
		{
			//fetch the original function addr using dlsym
			original_cuEGLStreamProducerConnect = (CUresult (*)(
			CUeglStreamConnection *, 
			EGLStreamKHR, 
			EGLint, 
			EGLint)
			) dlsym(original_libcuda_handle, "cuEGLStreamProducerConnect");
			fprintf(stderr, "original_cuEGLStreamProducerConnect:%p\n", original_cuEGLStreamProducerConnect);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEGLStreamProducerConnect():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEGLStreamProducerConnect(
		conn, 
		stream, 
		width, 
		height
		);
	}
}