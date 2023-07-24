#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLUnmapBufferObject)(
GLuint
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLUnmapBufferObject(GLuint buffer) {
		fprintf(stderr, "cuGLUnmapBufferObject()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLUnmapBufferObject)
		{
			//fetch the original function addr using dlsym
			original_cuGLUnmapBufferObject = (CUresult (*)(
			GLuint)
			) dlsym(original_libcuda_handle, "cuGLUnmapBufferObject");
			fprintf(stderr, "original_cuGLUnmapBufferObject:%p\n", original_cuGLUnmapBufferObject);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLUnmapBufferObject():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLUnmapBufferObject(
		buffer
		);
	}
}