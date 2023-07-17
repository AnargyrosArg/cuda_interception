import torch

def forward(x):
    o = x + 1.0
    o = o.relu()
    return o

shape = (2, 32, 128, 512)
input = torch.rand(*shape).cuda()
t = torch.jit.script(forward)

with torch.jit.fuser("fuser2"):
    for k in range(15):
        o = t(input)