#!/usr/bin/python

import unittest
import logging
import logging.config

from scheduler.scheduler import Scheduler
from scheduler.processor import Processor
from scheduler.jobqueue import JobQueue
from scheduler.processormonitor import ProcessorMonitor
from ui.plot import Plot
from verifier.verifier import Verifier
import util.utils as Utils

from matplotlib import pyplot

# Run all tasks from workload 1 and workload 2
def runTasks():

  log = logging.getLogger("main")
  
  # setup
  workloads = { 'workload1': { 'input':'input/workload1.yaml', 'schedulesummary':'output/schedule1.txt', 
                'schedule':'output/schedule1.yaml', 'executions':'output/executions1.yaml' },
                'workload2': { 'input':'input/workload2.yaml', 'schedulesummary':'output/schedule2.txt', 
                'schedule':'output/schedule2.yaml', 'executions':'output/executions2.yaml' } }

  for id,workload in workloads.iteritems():
    # create schedule and run it
    input = workload['input']
    schedulesummary = workload['schedulesummary']
    schedule = workload['schedule']
    executions = workload['executions']
    log.info("")
    log.info("##############################################")
    log.info("# Running " + input)
    log.info("##############################################")
    log.info("")
    proc = Processor()
    w = Utils.loadYaml(input)
    pm = ProcessorMonitor(w)
    proc.addMonitor(pm)
    sc = Scheduler(w, proc)
    sc.run()
    Utils.dumpSchedule(sc.getSchedule(), schedulesummary)
    Utils.dumpYaml(sc.getSchedule(), schedule)
    Utils.dumpYaml(pm.getResults(), executions)

    # plot the schedule
    p = Plot(sc.getSchedule(), pm.getResults(), sc.getH(), sc.getI(), id)
    p.create()
    
    # verify the schedule
    v = Verifier(Utils.loadYaml(executions), sc.getH(), input)
    v.verifySchedule()
    
    # show the plot
    p.show() 
  
# Main starts here
if __name__ == '__main__':
  logging.config.fileConfig("conf/info.conf")
  runTasks()
