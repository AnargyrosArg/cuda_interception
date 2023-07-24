#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLUnregisterBufferObject)(
GLuint
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLUnregisterBufferObject(GLuint buffer) {
		fprintf(stderr, "cuGLUnregisterBufferObject()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLUnregisterBufferObject)
		{
			//fetch the original function addr using dlsym
			original_cuGLUnregisterBufferObject = (CUresult (*)(
			GLuint)
			) dlsym(original_libcuda_handle, "cuGLUnregisterBufferObject");
			fprintf(stderr, "original_cuGLUnregisterBufferObject:%p\n", original_cuGLUnregisterBufferObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLUnregisterBufferObject():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLUnregisterBufferObject(
		buffer
		);
	}
}