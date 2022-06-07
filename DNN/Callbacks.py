import os
from DNNTools.CallBacksBase import PlotOnTrainingBase
from DNNTools.PlottingFunctions import plot_losses, plot_rocs

class PlotOnTraining(PlotOnTrainingBase):
    def __init__(self, modelPath, metrics_to_plot=['loss', 'categorical_accuracy'], eachEpoch=False):
        PlotOnTrainingBase.__init__(self, modelPath=modelPath, eachEpoch=eachEpoch)
        self.metrics_to_plot = metrics_to_plot

    def MakePlots(self):
        for metric in self.metrics_to_plot:
            plot_losses(self.history, mode=metric, name=os.path.join(self.modelPath,'training_%s' % (metric)))
