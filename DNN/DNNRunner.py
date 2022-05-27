import os
from printing_utils import *
from utils import ensureDirectory
from DNNTools.DNNRunnerBase import *
from ConvertRootToInputs import ConvertRootToInputs
from PreprocessInputs import PreprocessInputs
from Plotter import Plotter
from Training import Training
# import PlotInputs
# import TrainNetwork


class DNNRunner(DNNRunnerBase):
    def __init__(self, dnnparameters, year, analysisname, input_base_path, output_base_path, plotprefix, samples, colors=['C0', 'C3', 'C1', 'C2', 'C4']):
        self.dnnparameters = dnnparameters
        self.year = year
        self.input_base_path  = input_base_path.replace('year',self.year)
        self.output_base_path = output_base_path.replace('year',self.year)
        # self.analysisname  = analysisname
        self.samples = dict((key.replace('year',self.year), value) for (key, value) in samples.items())
        DNNRunnerBase.__init__(self, self.dnnparameters, self.year, plotprefix, self.samples, colors)

        self.DefinePaths()
        self.CreateConverter()
        self.CreateInputProcessor()
        self.CreatePlotter()
        self.CreateTraining()
        # self.LoadInputs()
        # print('input_train', len(self.input_train))
        # print('input_val', len(self.input_val))
        # print('input_test', len(self.input_test))
        # print('label_train', len(self.label_train))
        # print('label_val', len(self.label_val))
        # print('label_test', len(self.label_test))
        # print('weights_train', len(self.weights_train))
        # print('weights_val', len(self.weights_val))
        # print('weights_test', len(self.weights_test))
        # self.PreprocessInputs  = PreprocessInputs
        # PreprocessInputsOld(self, None)

        # self.PlotInputs  = PlotInputs.PlotInputs
        # self.TrainNetwork  = TrainNetwork.TrainNetwork

    def DefinePaths(self):
        filepath_root    = self.input_base_path
        filepath_raw     = os.path.join(self.input_base_path,  'DNN', 'PandasRawInputs')
        filepath_preproc = os.path.join(self.input_base_path,  'DNN', 'PandasProcessedInputs')
        filepath_input   = os.path.join(self.input_base_path,  'DNN', 'input')
        filepath_result  = os.path.join(self.output_base_path, 'data')
        filepath_plots   = os.path.join(self.output_base_path, 'plotstest')
        self.DefinePathsBase(filepath_root, filepath_raw, filepath_preproc, filepath_input, filepath_result, filepath_plots)

    def CreateConverter(self):
        self.ConvertRootToInputs = ConvertRootToInputs(self.filepath_root, self.filepath_raw, samples=self.samples)

    def CreateInputProcessor(self):
        self.PreprocessInputs = PreprocessInputs(self.filepath_raw, self.filepath_preproc, samples=self.samples, classes=self.dnnparameters['classes'])

    def CreatePlotter(self):
        self.Plotter = Plotter(self.filepath_preproc, os.path.join(self.filepath_plots, 'DNNInputDistributions'), classes=self.dnnparameters['classes'])

    def CreateTraining(self):
        self.Training = Training(DNNparams=self.dnnparameters, inputdir=self.filepath_preproc, outputdir=os.path.join(self.filepath_result, 'DNNModels', 'base'), plotdir=os.path.join(self.filepath_plots, 'DNNPerformancePlots', 'base'))
