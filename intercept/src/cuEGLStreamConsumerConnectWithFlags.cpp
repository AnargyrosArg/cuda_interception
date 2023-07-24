#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEGLStreamConsumerConnectWithFlags)(
CUeglStreamConnection *, 
EGLStreamKHR, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEGLStreamConsumerConnectWithFlags(CUeglStreamConnection *conn, EGLStreamKHR stream, unsigned int flags) {
		fprintf(stderr, "cuEGLStreamConsumerConnectWithFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEGLStreamConsumerConnectWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cuEGLStreamConsumerConnectWithFlags = (CUresult (*)(
			CUeglStreamConnection *, 
			EGLStreamKHR, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuEGLStreamConsumerConnectWithFlags");
			fprintf(stderr, "original_cuEGLStreamConsumerConnectWithFlags:%p\n", original_cuEGLStreamConsumerConnectWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEGLStreamConsumerConnectWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEGLStreamConsumerConnectWithFlags(
		conn, 
		stream, 
		flags
		);
	}
}