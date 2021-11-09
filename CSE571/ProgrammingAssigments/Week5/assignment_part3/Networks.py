import torch
import torch.nn as nn
import numpy as np 

from Data_Loaders import Data_Loaders

class Action_Conditioned_FF(nn.Module):
    def __init__(self, input_size=6, output_size=1):
# STUDENTS: __init__() must initiatize nn.Module and define your network's
# custom architecture
        super(Action_Conditioned_FF, self).__init__()
        self.hidden_layer = nn.Linear(input_size, 6)
        self.output_layer = nn.Linear(6, output_size)
        self.relu = nn.ReLU()

    def forward(self, input):
# STUDENTS: forward() must complete a single forward pass through your network
# and return the output which should be a tensor
        hidden = self.relu(self.hidden_layer(input))
        output = self.output_layer(hidden)
        return output


    def evaluate(self, model, test_loader, loss_function):
# STUDENTS: evaluate() must return the loss (a value, not a tensor) over your testing dataset. Keep in
# mind that we do not need to keep track of any gradients while evaluating the
# model. loss_function will be a PyTorch loss function which takes as argument the model's
# output and the desired output.
        loss = 0
        for _, sample in enumerate(test_loader):
            model_output = self.forward(torch.tensor(sample['input'], dtype=torch.float32))
            loss += float(loss_function(model_output, torch.tensor(sample['label'], dtype=torch.float32)))
        return loss/len(test_loader)

def main():
    model = Action_Conditioned_FF()
    print(model.evaluate(model, Data_Loaders(16).train_loader, nn.MSELoss()))

if __name__ == '__main__':
    main()
