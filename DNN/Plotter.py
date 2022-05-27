from DNNTools.PlotterBase import *

class Plotter(PlotterBase):
    def __init__(self, inputdir, outdir, classes):
        PlotterBase.__init__(self, inputdir=inputdir, outdir=outdir)
        self.classes = classes

    def DefineClasses(self):
        return self.classes
