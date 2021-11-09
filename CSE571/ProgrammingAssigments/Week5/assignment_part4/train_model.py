from Data_Loaders import Data_Loaders
from Networks import Action_Conditioned_FF

import torch
import torch.nn as nn
import matplotlib.pyplot as plt


def train_model(no_epochs):

    batch_size = 16
    data_loaders = Data_Loaders(batch_size)
    model = Action_Conditioned_FF()
    loss_function = nn.MSELoss()

    losses = []
    min_loss = model.evaluate(model, data_loaders.test_loader, loss_function)
    losses.append(min_loss)

    learning_rate = 0.00001
    optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)
    last_loss_delta = None

    for epoch_i in range(no_epochs):
        model.train()
        for idx, sample in enumerate(data_loaders.train_loader): # sample['input'] and sample['label']
            optimizer.zero_grad()
            output = model(torch.tensor(sample['input'], dtype = torch.float32))
            loss = loss_function(output, torch.tensor(sample['label'], dtype = torch.float32))
            loss.backward()
            optimizer.step()
        losses.append(model.evaluate(model, data_loaders.test_loader, loss_function))
        if len(losses) == 2:
            last_loss_delta = losses[-2] - losses[-1]
        elif len(losses) > 2:
            new_delta = losses[-2] - losses[-1]
            percentage = new_delta / last_loss_delta 
            print("New delta is {}% of previous delta. New loss {}".format(percentage * 100, losses[-1]))
            last_loss_delta = new_delta
            if 85 < (percentage * 100) < 100:
                print("Ending early at epoch {}".format(epoch_i))
                break
    torch.save(model.state_dict(), 'saved/saved_model.pkl', _use_new_zipfile_serialization=False)
    test = model.evaluate(model, data_loaders.test_loader, loss_function)
    print(losses)
    print(test)
    plt.plot(range(len(losses)),losses)
    plt.title('Test Loss')
    plt.xlabel('Epochs')
    plt.ylabel('Loss (MSE)')
    plt.xticks(range(len(losses)))
    plt.show()
    

if __name__ == '__main__':
    no_epochs = 100
    train_model(no_epochs)
