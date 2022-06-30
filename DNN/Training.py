import numpy as np
import pandas as pd
from DNNTools.TrainingBase import *
from DNNTools.Models import SequentialModel
from DNNTools.functions_dnn import classes_to_str
from Callbacks import PlotOnTraining
from utils import ensureDirectory
from keras.callbacks import History, ModelCheckpoint, ReduceLROnPlateau, Callback
from DNNTools.DNNutils import SavePandas, LoadPandas

class Training(TrainingBase):
    def __init__(self, DNNparams={}, inputdir='', modelpath='', plotdir='', predictiondir=''):
        TrainingBase.__init__(self, DNNparams=DNNparams, inputdir=inputdir, outputdir=modelpath, do_weights=True)
        self.classes = DNNparams['classes']
        self.plotdir=plotdir
        self.predictiondir=predictiondir
        self.modelpath=modelpath
        ensureDirectory(self.plotdir)
        ensureDirectory(self.predictiondir)
        ensureDirectory(self.modelpath)


    def LoadInputs(self):
        self.inputs = {}
        self.labels = {}
        self.weights = {}
        self.index = {}
        for mode in ['train','val','test']:
            self.inputs[mode]  = LoadPandas(os.path.join(self.inputdir, 'input_%s_%s.csv' %(mode, self.frac) )).to_numpy()
            self.labels[mode]  = np.load(os.path.join(self.inputdir, 'label_%s_%s.npy' %(mode, self.frac) ))
            self.weights[mode] = LoadPandas(os.path.join(self.inputdir, 'weights_%s_%s.csv' %(mode, self.frac) ))
            self.index[mode] = self.weights[mode].index
            self.weights[mode] = self.weights[mode].to_numpy()[:,0]



    def SavePredictions(self):
        if not hasattr(self, 'predictions') or self.predictions == {}:
            self.Predict()
        for mode in self.predictions:
            SavePandas(self.predictions[mode], os.path.join(self.predictiondir, 'prediction_%s_%s.csv' % (mode, self.frac) ))



    def DefineCallbacks(self):

        history = History()
        filepath = os.path.join(self.modelpath, 'bestmodel_loss.h5')
        modelCheckpoint_loss_best = ModelCheckpoint(filepath=os.path.join(self.modelpath, 'bestmodel_loss.h5'), monitor='val_loss', save_weights_only=False, mode='min', period=1, verbose=0, save_best_only=True)
        modelCheckpoint_acc_best  = ModelCheckpoint(filepath=os.path.join(self.modelpath, 'bestmodel_acc.h5'),  monitor='val_categorical_accuracy', save_weights_only=False, mode='max', period=1, verbose=0,  save_best_only=True)
        PlotTraining = PlotOnTraining(modelPath=self.plotdir, eachEpoch=True)

        callbacks = []
        callbacks.append(history)
        callbacks.append(modelCheckpoint_loss_best)
        callbacks.append(modelCheckpoint_acc_best)
        callbacks.append(PlotTraining)
        self.callbacks = callbacks

    def MakeModel(self):
        params = self.DNNparams
        defaultparams = {}
        defaultparams['activation'] = 'relu'
        defaultparams['kernel_initializer'] = 'glorot_normal'
        defaultparams['bias_initializer'] = 'ones'
        defaultparams['activation_last'] = 'softmax'
        defaultparams['optimizer'] = 'adam' #'adamax'
        defaultparams['metrics'] = ['categorical_accuracy']
        params.update(defaultparams)
        self.model = SequentialModel(input_shape = (self.inputs['train'].shape[1],), output_shape=self.labels['train'].shape[1], params=params)
