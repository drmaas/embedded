import unittest
import logging
import logging.config

from scheduler import Scheduler
from processor import Processor
from jobqueue import JobQueue
from processormonitor import ProcessorMonitor
import util.utils as Utils

# Unit tests for the program modules
class Test(unittest.TestCase):

  # Data setup
  def setUp(self):
    self.yaml1 = "../input/workload1.yaml"
    self.yaml2 = "../input/workload2.yaml"
    self.y1 = Utils.loadYaml(self.yaml1)
    self.y2 = Utils.loadYaml(self.yaml2)
    self.proc = Processor()
    self.proc2 = Processor()
    self.pm = None
    self.pm2 = None
    self.jobqueue = JobQueue()
    self.s1 = "../output/schedule1.txt"
    self.s2 = "../output/schedule2.txt"
    self.e1 = "../output/executions1.yaml"
    self.e2 = "../output/executions2.yaml"

  # Data teardown
  def tearDown(self):pass

  # Ensure loading workload from xml works
  def testWorkload(self):
    print "testWorkload():"
    w = Utils.loadYaml(self.yaml1)
    self.assertEqual(len(w), 8)
    print
 
  # Ensure processor busy works - execute task1 (wcet 2)
  def testProcessorBusy(self):
    print "testProcessorBusy():"
    self.y1['task1']['id'] = "task1"
    self.proc.execute(self.y1['task1'])
    self.proc.incrTime()
    self.assertTrue(self.proc.busy())
    self.proc.incrTime()
    self.proc.incrTime()
    self.assertFalse(self.proc.busy())
    print

  # Test JobQueue
  def testJobQueue(self):
    print "testJobQueue():"
    for id,task in self.y1.iteritems():
      self.jobqueue.insertTask(task)
    for id,task in self.y1.iteritems():
      self.jobqueue.insertTask(task)
    self.assertEquals(self.jobqueue.length(), 16)
    while self.jobqueue.length() > 0:
      task = self.jobqueue.popHighestPriorityJob()
      self.assertIsNotNone(task)
    self.assertEquals(self.jobqueue.length(), 0)
    print 

  # Test scheduler    
  def testScheduler(self):
    print "testScheduler():"
    self.pm = ProcessorMonitor(self.y1)
    self.proc.addMonitor(self.pm)
    s = Scheduler(self.y1, self.proc)
    s.run()
    Utils.dumpSchedule(s.getSchedule(), self.s1)
    Utils.dumpYaml(self.pm.getResults(), self.e1)
    print
    #self.pm2 = ProcessorMonitor(self.y2)
    #self.proc2.addMonitor(self.pm2)
    #s2 = Scheduler(self.y2, self.proc2)
    #s2.run()
    #Utils.dumpSchedule(s2.getSchedule(), self.s2)
    #Utils.dumpYaml(self.pm2.getResults(), self.e2)

# Main starts here
if __name__ == '__main__':
  logging.config.fileConfig("../conf/debug.conf")
  unittest.main()
