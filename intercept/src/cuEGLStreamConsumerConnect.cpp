#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEGLStreamConsumerConnect)(
CUeglStreamConnection *, 
EGLStreamKHR
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEGLStreamConsumerConnect(CUeglStreamConnection *conn, EGLStreamKHR stream) {
		fprintf(stderr, "cuEGLStreamConsumerConnect()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEGLStreamConsumerConnect)
		{
			//fetch the original function addr using dlsym
			original_cuEGLStreamConsumerConnect = (CUresult (*)(
			CUeglStreamConnection *, 
			EGLStreamKHR)
			) dlsym(original_libcuda_handle, "cuEGLStreamConsumerConnect");
			fprintf(stderr, "original_cuEGLStreamConsumerConnect:%p\n", original_cuEGLStreamConsumerConnect);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEGLStreamConsumerConnect():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEGLStreamConsumerConnect(
		conn, 
		stream
		);
	}
}