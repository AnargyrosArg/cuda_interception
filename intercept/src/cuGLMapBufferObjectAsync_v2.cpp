#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLMapBufferObjectAsync_v2)(
CUdeviceptr *, 
size_t *, 
GLuint, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLMapBufferObjectAsync_v2(CUdeviceptr *dptr, size_t *size, GLuint buffer, CUstream hStream) {
		fprintf(stderr, "cuGLMapBufferObjectAsync_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLMapBufferObjectAsync_v2)
		{
			//fetch the original function addr using dlsym
			original_cuGLMapBufferObjectAsync_v2 = (CUresult (*)(
			CUdeviceptr *, 
			size_t *, 
			GLuint, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuGLMapBufferObjectAsync_v2");
			fprintf(stderr, "original_cuGLMapBufferObjectAsync_v2:%p\n", original_cuGLMapBufferObjectAsync_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLMapBufferObjectAsync_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLMapBufferObjectAsync_v2(
		dptr, 
		size, 
		buffer, 
		hStream
		);
	}
}