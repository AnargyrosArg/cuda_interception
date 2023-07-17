from sklearn.metrics import f1_score
from sklearn.metrics import accuracy_score
from sklearn.metrics import recall_score
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.model_selection import train_test_split
import torch
import torch.nn as nn
from torch.nn.functional import normalize
import numpy as np
import string
import copy
import math
import tqdm

from torchtext import data
from torchtext.vocab import Vectors, GloVe

from torchmetrics.classification import BinaryStatScores
from torchmetrics.classification import AUROC
from torchmetrics.classification import ROC

from transformers import BertTokenizer, BertModel


if(not torch.cuda.is_available):
  print("Cuda is not available!")
  exit()

"""#Load Data / Bert"""

#path for test set
test_set_path = None

if test_set_path is not None:
  test_df = pd.read_csv(test_set_path,delimiter='\t')

#read csv file, expecting tab separation
path = "./reviews.csv"

df = pd.read_csv(path,delimiter='\t')

#drop url column , it might confuse/overfit the model
df = df.drop(columns="url")

# Load pre-trained model tokenizer (vocabulary)
tokenizer = BertTokenizer.from_pretrained('bert-base-uncased')

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
bert = BertModel.from_pretrained('bert-base-uncased',output_hidden_states = True).to(device)

"""#Input Processing"""

def extract_vector(sentence):
  sentence = tokenizer.encode_plus(
            sentence ,
            None,
            padding='longest',
            truncation=True,
            add_special_tokens=True,
            return_attention_mask=True,
            max_length=512,
        )
  tokens_tensor = torch.tensor([sentence['input_ids']]).to(device)
  segments_tensors = torch.tensor([sentence['attention_mask']]).to(device)
  with torch.no_grad():
    outputs = bert(tokens_tensor, segments_tensors)
    hidden_states = outputs[2]
    token_vecs = hidden_states[-2][0]
    sentence_embedding = torch.mean(token_vecs, dim=0)
    return sentence_embedding

#converts a dataframe of reviews to a list of vectors so it can then be converted into a tensor
def df2vec_array(df):
  vec_array = []
  #for all reviews
  for review in df:
    vec_array.append(extract_vector(review.lower().translate(str.maketrans('', '', string.punctuation))))
  return vec_array

X = df['review']
#'Drop' all but Y's rating collumn
Y = df[['rating']].copy()

#classify ratings as positive or negative
Y.loc[df['rating'] <= 4, 'rating'] = 0
Y.loc[df['rating'] >= 7, 'rating'] = 1


if test_set_path is not None:
  #if test set has been specified , train set is entirety of X
  #and the validation set is the test set
  X_train = X
  Y_train = Y
  X_val = test_df['review']
  Y_val = test_df[['rating']].copy()
  Y_val.loc[test_df['rating'] <= 4, 'rating'] = 0
  Y_val.loc[test_df['rating'] >= 7, 'rating'] = 1
else:
  #split X and Y into training and validation partitions
  X_train, X_val, Y_train, Y_val = train_test_split(X,Y,test_size=0.10)

X_train,trashX, Y_train, trashY = train_test_split(X_train,Y_train,test_size=0.98)
X_val,trashX, Y_val, trashY = train_test_split(X_val,Y_val,test_size=0.98)


#convert dataframes to tensors
vec_array_train = df2vec_array(X_train)
vec_array_val = df2vec_array(X_val)

X_train = torch.stack(vec_array_train)
Y_train = torch.tensor(Y_train.values, dtype=torch.float).to(device)

X_val = torch.stack(vec_array_val)
Y_val = torch.tensor(Y_val.values, dtype=torch.float).to(device)

class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.linear = nn.Linear(768, 1)

    def forward(self,embedding):
      out = self.linear(embedding)
      return torch.sigmoid(out)

#code for early stopping from , since pytorch lib does not provide
#https://colab.research.google.com/github/jeffheaton/t81_558_deep_learning/blob/pytorch/t81_558_class_03_4_early_stop.ipynb#scrollTo=CAezCpVfOFAF&line=1&uniqifier=1

class EarlyStopping():
  def __init__(self, patience=5, min_delta=0.0, restore_best_weights=True):
    self.patience = patience
    self.min_delta = min_delta
    self.restore_best_weights = restore_best_weights
    self.best_model = None
    self.best_loss = None
    self.counter = 0
    self.status = ""

  def __call__(self, model, val_loss):
    if self.best_loss == None:
      self.best_loss = val_loss
      self.best_model = copy.deepcopy(model)
    elif self.best_loss - val_loss > self.min_delta:
      self.best_loss = val_loss
      self.counter = 0
      self.best_model.load_state_dict(model.state_dict())
    elif self.best_loss - val_loss < self.min_delta:
      self.counter += 1
      if self.counter >= self.patience:
        self.status = f"Stopped on {self.counter}"
        if self.restore_best_weights:
          model.load_state_dict(self.best_model.state_dict())
        return True
    self.status = f"{self.counter}/{self.patience}"
    return False

class BertDataset(torch.utils.data.Dataset):
    def __init__(self,x,y,tokenizer,max_length):
        super(BertDataset, self).__init__()
        self.data=list(x)
        self.target=list(y['rating'])
        self.tokenizer=tokenizer
        self.max_length=max_length
        self.bert = BertModel.from_pretrained('bert-base-uncased',output_hidden_states = True)
        self.bert.to(device)
        self.bert.eval()

    def __len__(self):
        return len(self.target)

    def __getitem__(self, index):
        text1 = self.data[index]
        inputs = self.tokenizer.encode_plus(
            text1 ,
            None,
            padding='longest',
            truncation=True,
            add_special_tokens=True,
            return_attention_mask=True,
            max_length=self.max_length,
        )
        ids = inputs["input_ids"]
        token_type_ids = inputs["token_type_ids"]
        mask = inputs["attention_mask"]
        with torch.no_grad():
          outputs = self.bert(torch.tensor([token_type_ids]).to(device),torch.tensor([mask]).to(device))
          hidden_states = outputs[2]
        token_vecs = hidden_states[-2][0]
        sentence_embedding = torch.mean(token_vecs, dim=0).to(device)
        return {
            'embedding': sentence_embedding,
            'target': torch.tensor(self.target[index], dtype=torch.float).to(device)
            }

#Initialise model
model = Net()
model.to(device)
#Define Hyperparameters
learning_rate = 0.1 * 1e-4
momentum = 0.9

#Data Loader parameters
batch_size = 64

#Early Stopping parameters
min_delta = 45/len(Y_val)
patience = 4

#Binary Cross Entropy Loss since we have a binary classification problem
loss_func = nn.BCELoss(reduction='sum')
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate,momentum=momentum)

#Initialise dataloader
#dataset = BertDataset(X_train,Y_train,tokenizer,max_length)
#dataloader = torch.utils.data.DataLoader(dataset, batch_size=batch_size, shuffle=True)
#val_dataset = BertDataset(X_val,Y_val,tokenizer,max_length)
#val_dataloader = torch.utils.data.DataLoader(val_dataset, batch_size=batch_size, shuffle=True)

#Initialise dataloader
dataset = torch.utils.data.TensorDataset(X_train,Y_train)
dataloader = torch.utils.data.DataLoader(dataset, batch_size=batch_size, shuffle=True)

val_dataset = torch.utils.data.TensorDataset(X_val,Y_val)
val_dataloader = torch.utils.data.DataLoader(dataset, batch_size=batch_size, shuffle=True)

es = EarlyStopping(patience=patience,min_delta=min_delta)

train_losses = []
val_losses = []


#model = torch.jit.script(model)

for epoch in range(30):
  batch_losses = []

  #set model to training mode
  model.train()
  for x_batch, y_batch in dataloader:
    #Delete previously stored gradients
    optimizer.zero_grad()
    #predict training data
    y_pred = model(x_batch)
    #calculate training loss
    train_loss = loss_func(y_pred, y_batch)
    batch_losses.append(train_loss.item())
    #Perform backpropagation starting from the loss calculated in this epoch
    train_loss.backward()
    #Update model's weights based on the gradients calculated during backprop
    optimizer.step()

  #predict validation set
  model.eval()
  with torch.no_grad():
    y_pred_val = model(X_val)
    y_pred_train = model(X_train)
  loss_val = loss_func(y_pred_val , Y_val)
  loss_train = loss_func(y_pred_train , Y_train)
  train_losses.append(loss_train.item()/len(Y_train))
  val_losses.append(loss_val.item()/len(Y_val))

  if es(model,loss_val):
    break

  #Printed validation loss is not normalized!
  print(f"Epoch {epoch:3}: Val Loss:{loss_val/len(X_val):20} Train Loss:{loss_train/len(X_train):20} F1:{f1_score(Y_val.flatten().tolist(),np.round(y_pred_val.flatten().tolist())):20} ES:{es.status}")
print(f"Epoch {epoch:3}: Val Loss:{loss_val/len(X_val):20} Train Loss:{loss_train/len(X_train):20} F1:{f1_score(Y_val.flatten().tolist(),np.round(y_pred_val.flatten().tolist())):20} ES:{es.status}")

"""#Metrics

"""

model.eval()
with torch.no_grad():
  y_pred = model(X_val)
  y_pred_train = model(X_train)

accu = accuracy_score(Y_val.flatten().tolist(),np.round(y_pred.flatten().tolist()))
rec = recall_score(Y_val.flatten().tolist(),np.round(y_pred.flatten().tolist()))
f1 = f1_score(Y_val.flatten().tolist(),np.round(y_pred.flatten().tolist()))

accu_train = accuracy_score(Y_train.flatten().tolist(),np.round(y_pred_train.flatten().tolist()))
rec_train = recall_score(Y_train.flatten().tolist(),np.round(y_pred_train.flatten().tolist()))
f1_train = f1_score(Y_train.flatten().tolist(),np.round(y_pred_train.flatten().tolist()))

print("Val Accuracy: " + str(accu))
print("Val Recall:   " + str(rec))
print("Val F1 Score: " + str(f1))

print("Train Accuracy: " + str(accu_train))
print("Train Recall:   " + str(rec_train))
print("Train F1 Score: " + str(f1_train))

"""#ROC Curve"""

roc = ROC(task="binary")
fpr, tpr, thresholds = roc(y_pred, Y_val.type(torch.int64))
fpr_train, tpr_train,thresholds_train = roc(y_pred_train, Y_train.type(torch.int64))

auroc = AUROC(task="binary")
AUC = auroc(y_pred,Y_val)
AUC_train = auroc(y_pred_train,Y_train)

#ROC curve
plt.plot(fpr.cpu(),tpr.cpu())
plt.plot(fpr_train.cpu(),tpr_train.cpu())

#plot diagonal
plt.plot([0,1],[0,1])

plt.ylim(ymin=0)
plt.legend(["ROC Curve Validation (AUC = " + str(AUC.item()) + " )","ROC Curve Train (AUC = " + str(AUC_train.item()) + " )","Random Chance"])

"""#Learning Curve"""

plt.plot(range(len(train_losses)),train_losses)
plt.plot(range(len(val_losses)),val_losses)

#plt.ylim(ymin=0)
plt.legend(["Train Loss","Val Loss"])