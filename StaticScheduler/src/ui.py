#!/usr/bin/python

from matplotlib.backends.backend_gtk3agg import FigureCanvasGTK3Agg as FigureCanvas
from matplotlib.backends.backend_gtk3 import NavigationToolbar2GTK3 as NavigationToolbar

import logging
import logging.config

from manager.workloadmanager import WorkloadManager

#import gtk
from gi.repository import Gtk as gtk

class StaticScheduler(gtk.Window):
    def __init__(self):
        super(StaticScheduler, self).__init__()
        
        logging.config.fileConfig("../conf/info.conf")
        
        # setup workload input data
        workloads = { 'workload1': { 'input':'../input/workload1.yaml', 'schedulesummary':'../output/schedule1.txt', 
                      'schedule':'../output/schedule1.yaml', 'executions':'../output/executions1.yaml' },
                      'workload2': { 'input':'../input/workload2.yaml', 'schedulesummary':'../output/schedule2.txt', 
                      'schedule':'../output/schedule2.yaml', 'executions':'../output/executions2.yaml' } }
        
        self.set_title("Static Scheduler")
        self.set_size_request(300, 200)
        self.set_position(gtk.WindowPosition.CENTER)
        self.connect("destroy", gtk.main_quit)
        
        fixed = gtk.Fixed()

        for i in range(1,3):
            wid = "workload"+str(i)
            label = "Analyze Workload "+str(i)
            title = "Workload "+str(i)
            workload = workloads[wid]
            quit = gtk.Button(label)
            quit.connect("clicked", self.on_clicked, title, wid, workload)
            quit.set_size_request(160, 35)
            fixed.put(quit, 70, 50*i)

        self.add(fixed)
        self.show_all()
        
    # run schedule, verify, show figure
    def on_clicked(self, widget, title, wid, workload):
        # run schedule
        wm = WorkloadManager(wid, workload)
        fig = wm.runAll()
        
        # create window
        win = gtk.Window()
        win.set_default_size(1024,768)
        win.set_title(title)
        
        # setup drawing area
        #sw = gtk.ScrolledWindow()
        canvas = FigureCanvas(fig)  # a Gtk.DrawingArea
        canvas.set_size_request(1024,768)
        #sw.add_with_viewport(canvas)
        vbox = gtk.VBox()
        win.add(vbox)
        #win.add(sw)
        #sw.set_border_width(10)
        vbox.pack_start(canvas, True, True, 0)
        
        # Create toolbar
        toolbar = NavigationToolbar(canvas, win)
        vbox.pack_start(toolbar, False, False, 0)
        
        win.show_all()

StaticScheduler()
gtk.main()
