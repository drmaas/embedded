import unittest
import logging
import logging.config

from plot import Plot

import util.utils as Utils

# Unit tests for the program modules
class Test(unittest.TestCase):

    # Data setup
    def setUp(self):
        self.schedulefile = "../output/schedule2.yaml"
        self.execfile = "../output/executions2.yaml"
        self.schedule = Utils.loadYaml(self.schedulefile)
        self.executions = Utils.loadYaml(self.execfile)

    # Data teardown
    def tearDown(self):pass

    # Ensure loading workload from xml works
    def testPlotSchedule(self):
        print "testPlotSchedule():"
        #p = Plot(self.schedule, self.executions, 80, 5, "Workoad 1")
        p = Plot(self.schedule, self.executions, 300, 10, "Workoad 2")
        p.addScheduleBar()
        p.addTaskBars()
        p.addLegend()
        p.addMiscInfo()
        #p.show()

    def testGtk(self):
        from gi.repository import Gtk

        from matplotlib.figure import Figure
        from matplotlib.backends.backend_gtk3agg import FigureCanvasGTK3Agg as FigureCanvas

        win = Gtk.Window()
        win.connect("delete-event", Gtk.main_quit )
        win.set_default_size(800,600)
        win.set_title("Embedding in GTK")

        p = Plot(self.schedule, self.executions, 300, 10, "Workoad 2")
        p.addScheduleBar()
        p.addTaskBars()
        p.addLegend()
        p.addMiscInfo()
        f = p.getFigure()

        sw = Gtk.ScrolledWindow()
        win.add (sw)

        # A scrolled window border goes outside the scrollbars and viewport
        sw.set_border_width (10)

        canvas = FigureCanvas(f)  # a Gtk.DrawingArea
        canvas.set_size_request(800,600)
        sw.add_with_viewport (canvas)
        
        win.show_all()
        Gtk.main()
        
        win2 = Gtk.Window()
        win2.connect("delete-event", Gtk.main_quit )
        win2.set_default_size(800,600)
        win2.set_title("Embedding in GTK 2")
        
        win2.show_all()
        Gtk.main()        

# Main starts here
if __name__ == '__main__':
    logging.config.fileConfig("../conf/debug.conf")
    unittest.main()


