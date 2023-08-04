import torch

model = torch.hub.load('pytorch/vision:v0.10.0', 'resnet18', pretrained=True).cuda()
opt_model = torch.compile(model, backend="aot_ts_nvfuser")
for i in range(5):
    out = model(torch.randn(1,3,64,64).cuda())

