#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamGetCaptureInfo_v2)(
cudaStream_t, 
enum cudaStreamCaptureStatus *, 
unsigned long long *, 
cudaGraph_t *, 
const cudaGraphNode_t **, 
size_t *
);
extern "C"
{
	cudaError_t cudaStreamGetCaptureInfo_v2(cudaStream_t stream, enum cudaStreamCaptureStatus *captureStatus_out, unsigned long long *id_out, cudaGraph_t *graph_out, const cudaGraphNode_t **dependencies_out, size_t *numDependencies_out) {
		fprintf(stderr, "===============\ncudaStreamGetCaptureInfo_v2()\n");
		char* __dlerror;
		if (!original_cudaStreamGetCaptureInfo_v2)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamGetCaptureInfo_v2 = (cudaError_t (*)(
			cudaStream_t, 
			enum cudaStreamCaptureStatus *, 
			unsigned long long *, 
			cudaGraph_t *, 
			const cudaGraphNode_t **, 
			size_t *)
			) dlsym(RTLD_NEXT, "cudaStreamGetCaptureInfo_v2");
			fprintf(stderr, "original_cudaStreamGetCaptureInfo_v2:%p\n", original_cudaStreamGetCaptureInfo_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamGetCaptureInfo_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamGetCaptureInfo_v2(
		stream, 
		captureStatus_out, 
		id_out, 
		graph_out, 
		dependencies_out, 
		numDependencies_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}