#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLUnmapBufferObjectAsync)(
GLuint, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLUnmapBufferObjectAsync(GLuint buffer, CUstream hStream) {
		fprintf(stderr, "cuGLUnmapBufferObjectAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLUnmapBufferObjectAsync)
		{
			//fetch the original function addr using dlsym
			original_cuGLUnmapBufferObjectAsync = (CUresult (*)(
			GLuint, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuGLUnmapBufferObjectAsync");
			fprintf(stderr, "original_cuGLUnmapBufferObjectAsync:%p\n", original_cuGLUnmapBufferObjectAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLUnmapBufferObjectAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLUnmapBufferObjectAsync(
		buffer, 
		hStream
		);
	}
}