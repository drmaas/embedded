import unittest
import logging
import logging.config

from scheduler.scheduler import Scheduler
from scheduler.processor import Processor
from scheduler.jobqueue import JobQueue
from scheduler.processormonitor import ProcessorMonitor
from verifier.verifier import Verifier
import util.utils as Utils

# Run all tasks from workload 1 and workload 2
def runTasks():

  # setup
  workloads = { 'workload1': { 'input':'input/workload1.yaml', 'schedule':'output/schedule1.txt', 'executions':'output/executions1.yaml' },
                'workload2': { 'input':'input/workload2.yaml', 'schedule':'output/schedule2.txt', 'executions':'output/executions2.yaml' } }

  for id,workload in workloads.iteritems():
    # create schedule and run it
    input = workload['input']
    schedule = workload['schedule']
    executions = workload['executions']
    print
    print "##############################################"
    print "# Running " + input
    print "##############################################"
    print
    proc = Processor()
    w = Utils.loadYaml(input)
    pm = ProcessorMonitor(w)
    proc.addMonitor(pm)
    sc = Scheduler(w, proc)
    sc.run()
    Utils.dumpSchedule(sc.getSchedule(), schedule)
    Utils.dumpYaml(pm.getResults(), executions)

    # verify the schedule
    v = Verifier(Utils.loadYaml(executions), sc.getH())
    v.verifySchedule()

# Main starts here
if __name__ == '__main__':
  logging.config.fileConfig("conf/info.conf")
  runTasks()
