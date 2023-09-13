# Description
Interception of CUDA Driver API calls (libcuda.so) and CUDA Runtime API.
Works for CUDA 11.7

# Compile

Go to /intercept and run 
    make -j

Then run with an executable LD_PRELOAD=/path/to/libintercept.so ./executable
The examples have a run.sh script or a Makefile run rule