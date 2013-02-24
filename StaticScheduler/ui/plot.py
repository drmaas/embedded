from matplotlib import pyplot, mpl

class Plot(object):

    # H is hyperperiod, I is interrupt
    def __init__(self, schedule, executions, H, I, name):
        self.schedule = schedule
        self.executions = executions
        self.H = H
        self.I = I
        self.name = name
        self.fig = pyplot.figure(num='Static Scheduling Tool',figsize=(10,5), dpi=160)
        self.fig.suptitle(self.name, fontsize=12)
       
    # add legend
    def addLegend(self):
        prop = mpl.font_manager.FontProperties(size='xx-small')
        rec1 = mpl.patches.Rectangle((.1,.1),.1,.1,color='r')
        rec2 = mpl.patches.Rectangle((.1,.1),.1,.1,color='b')
        rec3 = mpl.patches.Rectangle((.1,.1),.1,.1,color='g')
        legend = self.fig.legend( (rec1, rec2, rec3),
                        ('busy', 'slack', 'idle'), 'lower right',
                        prop=prop)
       
       
    # Add the busy idle schedule
    def addScheduleBar(self):
        length = len(self.schedule)
        colors = []
        for i in range(0, length):
            # setup this task and next
            t = self.schedule[i]
            if i < (length - 1):
                next_t = self.schedule[i+1]
            else:
                next_t = None
            
            # calculate run, wcet_idle, and true_idle
            run = t['et']
            wcet_idle = t['wcet'] - run
            if next_t != None:
                true_idle = next_t['start'] - (t['start'] + t['wcet'])
            else:
                true_idle = self.H - (t['start'] + t['wcet'])
                
            # add them to color map
            self.addColors(colors, 0, run, wcet_idle, true_idle)               
            
        # set up color bar
        self.createBar(colors, 'Schedule', 0.88)
        
    # add all task bars
    def addTaskBars(self):
        i = 0
        start_v_offset = 0.78
        for id, task in self.executions.iteritems():
            v_offset = start_v_offset - (i*0.09)
            self.addTaskBar(id, task, v_offset)
            i += 1
    
    # add a single task bar
    def addTaskBar(self, id, task, v_offset):
        colors = []
        times = task['times']
        length = len(times)
        wcet = task['wcet']
        period = task['period']
        for i in range(0,length):
            # get this execution of task
            time = times[i]
                
            # calculate run, wcet_idle, and true_idle    
            start = time['start']
            begin_idle = start - (i*period)
            run = time['duration']
            wcet_idle = wcet - run
            if i < (length - 1):
                true_idle = (i+1)*period - (start + wcet)
            else:
                true_idle =  self.H - (start + wcet)
                
            # add them to color map    
            self.addColors(colors, begin_idle, run, wcet_idle, true_idle)
                
        # set up color bar
        self.createBar(colors, id, v_offset)
        
    # add colors to color array
    def addColors(self, colors, begin_idle, run, wcet_idle, true_idle):
        for j in range(0, begin_idle):
            colors.append('g')
        for j in range(0, run):
            colors.append('r')
        for j in range(0, wcet_idle):
            colors.append('b')
        for j in range(0, true_idle):
            colors.append('g')  
            
    # draw a color bar v_offset from bottom
    def createBar(self, colors, label, v_offset):
        ax = self.fig.add_axes([0.025, v_offset, 0.95, 0.05])   
        cmap = mpl.colors.ListedColormap(colors)
        bounds = range(0,self.H+1)
        ticks = range(0,self.H+1,self.I)
        norm = mpl.colors.BoundaryNorm(bounds, cmap.N)
        bar = mpl.colorbar.ColorbarBase(ax, cmap=cmap,
                                            norm=norm,
                                            boundaries=bounds,
                                            extend='neither',
                                            ticks=ticks,
                                            spacing='proportional',
                                            orientation='horizontal',
                                            drawedges='true')
        ax.set_ylabel(label, fontsize='xx-small')                
        ax.set_xticklabels(ticks,fontsize='xx-small')
        
    # add misc info
    def addMiscInfo(self):
        self.fig.text(0.5, 0.05, 'Time', fontsize=12)
        self.fig.text(.65, 0.05, 'Hyperperiod='+str(self.H), fontsize=12)
        self.fig.text(.65, 0.015, 'Period Interrupt='+str(self.I), fontsize=12)
        
    # show the plot
    def show(self):
        pyplot.show()
       
    # SAMPLE NOT USED
    def samplePlot(self):
        # Make a figure and axes with dimensions as desired.
        
        fig = pyplot.figure(num="Schedule",figsize=(10,5), dpi=160)
        # figsize is width, height
        # left,bottom,width,height
        ax1 = fig.add_axes([0.025, 0.80, 0.95, 0.15])
        ax2 = fig.add_axes([0.025, 0.475, 0.95, 0.15])
        ax3 = fig.add_axes([0.025, 0.15, 0.95, 0.15])

        # Set the colormap and norm to correspond to the data for which
        # the colorbar will be used.
        cmap = mpl.cm.cool
        norm = mpl.colors.Normalize(vmin=5, vmax=10)

        # ColorbarBase derives from ScalarMappable and puts a colorbar
        # in a specified axes, so it has everything needed for a
        # standalone colorbar.  There are many more kwargs, but the
        # following gives a basic continuous colorbar with ticks
        # and labels.
        cb1 = mpl.colorbar.ColorbarBase(ax1, cmap=cmap,
                                           norm=norm,
                                           orientation='horizontal')
        cb1.set_label('Some Units')

        # The second example illustrates the use of a ListedColormap, a
        # BoundaryNorm, and extended ends to show the "over" and "under"
        # value colors.
        cmap = mpl.colors.ListedColormap(['r', 'g', 'b', 'c'])
        #cmap.set_over('0.25')
        #cmap.set_under('0.75')

        # If a ListedColormap is used, the length of the bounds array must be
        # one greater than the length of the color list.  The bounds must be
        # monotonically increasing.
        #bounds = [1, 2, 4, 7, 8]
        bounds = range(0,81,20)
        ticks = range(0,81,10)
        norm = mpl.colors.BoundaryNorm(bounds, cmap.N)
        #norm = mpl.colors.Normalize(vmin=0, vmax=80)
        cb2 = mpl.colorbar.ColorbarBase(ax2, cmap=cmap,
                                             norm=norm,
                                             # to use 'extend', you must
                                             # specify two extra boundaries:
                                             boundaries=bounds,
                                             extend='neither',
                                             ticks=ticks,
                                             spacing='proportional',
                                             orientation='horizontal')
        cb2.set_label('Discrete intervals, some other units')

        # The third example illustrates the use of custom length colorbar
        # extensions, used on a colorbar with discrete intervals.
        cmap = mpl.colors.ListedColormap([[0., .4, 1.], [0., .8, 1.],
            [1., .8, 0.], [1., .4, 0.]])
        cmap.set_over((1., 0., 0.))
        cmap.set_under((0., 0., 1.))

        bounds = [-1., -.5, 0., .5, 1.]
        norm = mpl.colors.BoundaryNorm(bounds, cmap.N)
        cb3 = mpl.colorbar.ColorbarBase(ax3, cmap=cmap,
                                             norm=norm,
                                             boundaries=[-10]+bounds+[10],
                                             extend='both',
                                             ticks=bounds,
                                             spacing='uniform',
                                             orientation='horizontal')
        cb3.set_label('Custom extension lengths, some other units')

        pyplot.show()
