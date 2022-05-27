import numpy as np
import pandas as pd
from DNNTools.TrainingBase import *
from DNNTools.Models import SequentialModel
from DNNTools.functions_dnn import classes_to_str
from DNNTools.CallBacksBase import PlotOnTrainingBase
from utils import ensureDirectory

class Training(TrainingBase):
    def __init__(self, DNNparams={}, inputdir='', outputdir='', plotdir=''):
        TrainingBase.__init__(self, DNNparams=DNNparams, inputdir=inputdir, outputdir=outputdir, do_weights=True)
        self.classes = DNNparams['classes']
        self.plotdir=plotdir
        ensureDirectory(self.plotdir)


    def LoadInputs(self):
        self.inputs = {}
        self.labels = {}
        self.weights = {}
        for mode in ['train','val','test']:
            self.inputs[mode]  = pd.read_pickle(os.path.join(self.inputdir, classes_to_str(self.classes), 'input_%s_%s.pkl' %(mode,self.frac) )).to_numpy()
            self.labels[mode]  = np.load(os.path.join(self.inputdir, classes_to_str(self.classes), 'label_%s_%s.npy' %(mode,self.frac) ))
            self.weights[mode] = pd.read_pickle(os.path.join(self.inputdir, classes_to_str(self.classes), 'weights_%s_%s.pkl' %(mode,self.frac) )).to_numpy()


    def DefineCallbacks(self):
        # checkpointer_everymodel = ModelCheckpoint(filepath=os.path.join(modelpath, 'model_epoch{epoch:02d}.h5'), verbose=1, save_best_only=False, period=1)
        # checkpoint_bestmodel = ModelCheckpoint(filepath=os.path.join(modelpath, 'model_best.h5'), monitor='val_loss', verbose=0, save_best_only=True, save_weights_only=False, mode='min', period=1)
        # earlystopping = EarlyStopping(monitor='val_loss', min_delta=0.005, patience=20, verbose=0, mode='min', baseline=None, restore_best_weights=True)
        # LRreducer = ReduceLROnPlateau(monitor='val_loss', factor=0.5, patience=50, min_delta=0.001, mode='min')
        # reduceLROnPlateau = ReduceLROnPlateau(monitor='val_loss', factor=0.5, patience=1, min_lr=0.001, cooldown=10)

        self.callbacks = DefineCallbacksBase(self.plotdir)
        PlotTraining = PlotOnTrainingBase(modelPath=self.plotdir, eachEpoch=True)
        self.callbacks.append(PlotTraining)

    def MakeModel(self):
        self.model = SequentialModel(input_shape = (self.inputs['train'].shape[1],), output_shape=self.labels['train'].shape[1], params=self.DNNparams)
