import os
from printing_utils import *
from utils import ensureDirectory
from DNNTools.DNNRunnerBase import *
from ConvertRootToInputs import ConvertRootToInputs
from PreprocessInputs import PreprocessInputs
from Plotter import Plotter
from Training import Training
from DNNTools.functions_dnn import parameters_to_tag


class DNNRunner(DNNRunnerBase):
    def __init__(self, dnnparameters, year, analysisname, input_base_path, output_base_path, samples):
        self.dnnparameters = dnnparameters
        self.year = year
        self.input_base_path  = input_base_path.replace('year',self.year)
        self.output_base_path = output_base_path.replace('year',self.year)
        self.samples = dict((key.replace('year',self.year), value) for (key, value) in samples.items())
        self.modeltag = parameters_to_tag(self.dnnparameters)
        self.classtag = classes_to_str(self.dnnparameters['classes'])
        DNNRunnerBase.__init__(self, self.dnnparameters, self.samples)

        self.DefinePaths()
        self.CreateConverter()
        self.CreateInputProcessor()
        self.CreatePlotter()
        self.CreateTraining()


    def DefinePaths(self):

        paths = {
            'root' :                self.input_base_path,
            'raw' :                 os.path.join(self.input_base_path,  'DNN', 'PandasRawInputs'),
            'preproc' :             os.path.join(self.input_base_path,  'DNN', 'PandasProcessedInputs'),
            'result' :              os.path.join(self.output_base_path, 'data'),
            'plots_inputdistributions' : os.path.join(self.output_base_path, 'plotstest', 'DNNPlots', 'InputDistributions', self.classtag),
            'plots_performance' :   os.path.join(self.output_base_path, 'plotstest', 'DNNPlots', 'Performance',   self.modeltag),
            'predictions' :         os.path.join(self.input_base_path, 'DNN', 'PandasPredictions', self.modeltag)
        }

        paths.update({'model': os.path.join(paths['result'], 'DNNModels', self.modeltag)})
        self.DefinePathsBase(**paths)

    def CreateConverter(self):
        self.ConvertRootToInputs = ConvertRootToInputs(self.filepath['root'], self.filepath['raw'], samples=self.samples, run_on='8')

    def CreateInputProcessor(self):
        self.PreprocessInputs = PreprocessInputs(self.filepath['raw'], self.filepath['preproc'], samples=self.samples, classes=self.dnnparameters['classes'], runonfraction=self.dnnparameters['runonfraction'])

    def CreatePlotter(self):
        self.Plotter = Plotter(classes=self.dnnparameters['classes'], modelpath=self.filepath['model'])

    def CreateTraining(self):
        self.Training = Training(DNNparams=self.dnnparameters, inputdir=self.filepath['preproc'], modelpath=self.filepath['model'], plotdir=self.filepath['model'], predictiondir=self.filepath['predictions'])
