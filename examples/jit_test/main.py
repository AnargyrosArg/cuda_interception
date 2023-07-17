
import torchvision
import torch
from time import perf_counter
import numpy as np

def timer(f,*args):   
    start = perf_counter()
    f(*args)
    return (1000 * (perf_counter() - start))


torch.backends.cudnn.enabled = False


x_ft = torch.rand(1,3, 224,224)
with torch.jit.fuser("fuser2"):
    # Example 1.2 Pytorch gpu version
    model_ft_gpu = torchvision.models.resnet18(weights='ResNet18_Weights.DEFAULT').cuda()
    x_ft_gpu = x_ft.cuda()
    model_ft_gpu.eval()
    print(np.mean([timer(model_ft_gpu,x_ft_gpu) for _ in range(25)]))


    # Example 2.2 torch.jit.script gpu version
    script_cell_gpu = torch.jit.script(model_ft_gpu, (x_ft_gpu))
    print(type(script_cell_gpu))
    print(np.mean([timer(script_cell_gpu,x_ft_gpu) for _ in range(25)]))