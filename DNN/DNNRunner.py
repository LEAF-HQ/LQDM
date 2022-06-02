import os
from printing_utils import *
from utils import ensureDirectory
from DNNTools.DNNRunnerBase import *
from ConvertRootToInputs import ConvertRootToInputs
from PreprocessInputs import PreprocessInputs
from Plotter import Plotter
from Training import Training
from DNNTools.functions_dnn import parameters_to_tag
# import PlotInputs
# import TrainNetwork


class DNNRunner(DNNRunnerBase):
    def __init__(self, dnnparameters, year, analysisname, input_base_path, output_base_path, samples):
        self.dnnparameters = dnnparameters
        self.year = year
        self.input_base_path  = input_base_path.replace('year',self.year)
        self.output_base_path = output_base_path.replace('year',self.year)
        # self.analysisname  = analysisname
        self.samples = dict((key.replace('year',self.year), value) for (key, value) in samples.items())
        DNNRunnerBase.__init__(self, self.dnnparameters, self.year, self.samples)

        self.DefinePaths()
        self.CreateConverter()
        self.CreateInputProcessor()
        self.CreatePlotter()
        self.CreateTraining()
        # self.LoadInputs()
        # self.PreprocessInputs  = PreprocessInputs
        # PreprocessInputsOld(self, None)

        # self.PlotInputs  = PlotInputs.PlotInputs
        # self.TrainNetwork  = TrainNetwork.TrainNetwork

    def DefinePaths(self):
        filepath_root    = self.input_base_path
        filepath_raw     = os.path.join(self.input_base_path,  'DNN', 'PandasRawInputs')
        filepath_preproc = os.path.join(self.input_base_path,  'DNN', 'PandasProcessedInputs')
        filepath_result  = os.path.join(self.output_base_path, 'data')
        filepath_plots   = os.path.join(self.output_base_path, 'plotstest')
        filepath_predictions  = os.path.join(self.input_base_path, 'DNN', 'PandasPredictions')
        self.DefinePathsBase(filepath_root=filepath_root, filepath_raw=filepath_raw, filepath_preproc=filepath_preproc, filepath_result=filepath_result, filepath_plots=filepath_plots, filepath_predictions=filepath_predictions)

    def CreateConverter(self):
        self.ConvertRootToInputs = ConvertRootToInputs(self.filepath['filepath_root'], self.filepath['filepath_raw'], samples=self.samples)

    def CreateInputProcessor(self):
        self.PreprocessInputs = PreprocessInputs(self.filepath['filepath_raw'], self.filepath['filepath_preproc'], samples=self.samples, classes=self.dnnparameters['classes'], runonfraction=self.dnnparameters['runonfraction'])

    def CreatePlotter(self):
        self.Plotter = Plotter(self.filepath['filepath_preproc'], os.path.join(self.filepath['filepath_plots'], 'DNNInputDistributions'), classes=self.dnnparameters['classes'])

    def CreateTraining(self):
        classtag = parameters_to_tag(self.dnnparameters)
        print classtag
        self.Training = Training(DNNparams=self.dnnparameters, inputdir=self.filepath['filepath_preproc'], outputdir=os.path.join(self.filepath['filepath_result'], 'DNNModels', classtag), plotdir=os.path.join(self.filepath['filepath_plots'], 'DNNPerformancePlots', classtag), predictiondir=os.path.join(self.filepath['filepath_predictions'], classtag))
