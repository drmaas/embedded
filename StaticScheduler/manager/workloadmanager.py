from scheduler.scheduler import Scheduler
from scheduler.processor import Processor
from scheduler.jobqueue import JobQueue
from scheduler.processormonitor import ProcessorMonitor
from ui.plot import Plot
from verifier.verifier import Verifier
import util.utils as Utils

import logging

class WorkloadManager(object):

  def __init__(self, wid, workload):
  
     self.log = logging.getLogger("WorkloadManager")
     
     self.wid = wid
     self.workload = workload
     
  # run schedule, verify it, and create plot figure
  def runAll(self):
    # create schedule and run it
    input = self.workload['input']
    schedulesummary = self.workload['schedulesummary']
    schedule = self.workload['schedule']
    executions = self.workload['executions']
    self.log.info("")
    self.log.info("##############################################")
    self.log.info("# Running " + input)
    self.log.info("##############################################")
    self.log.info("")
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
    p = Plot(sc.getSchedule(), pm.getResults(), sc.getH(), sc.getI(), self.wid)
    p.create()
    
    # verify the schedule
    v = Verifier(Utils.loadYaml(executions), sc.getH(), input)
    v.verifySchedule()
    
    return p.getFigure()
  
