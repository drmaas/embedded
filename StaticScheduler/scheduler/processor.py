from processormonitor import ProcessorMonitor

# Class to model a processor
class Processor(object):

  def __init__(self):
    self.currtime = 0
    self.taskstarttime = 0
    self.taskendtime = 0
    self.monitors = []
    self.taskid = ''
    self.exectime = 0

  # execute just sets the task end point, doesn't actually do anything real
  # currently all tasks take a random time between 1 and worst-case execution time units
  def execute(self, task, etFunc):
    self.taskid = task['id']
    self.taskstarttime = self.currtime
    self.exectime = etFunc(task['wcet']);
    self.taskendtime = self.currtime + self.exectime

  # check if the processor is currently executing
  def busy(self):
    if self.currtime < self.taskendtime:
      return True
    else:
      return False

  # increment the processor's time (clock) and check to update actual execution time of task
  def incrTime(self):
    self.currtime += 1
    if self.currtime == self.taskendtime:
      for monitor in self.monitors:
        monitor.addExecution(self.taskid, {'start':self.taskstarttime, 'duration':self.taskendtime-self.taskstarttime})

  # set execution results data structure
  def addMonitor(self, monitor):
    self.monitors.append(monitor)

  # return results so we can do stuff with them
  def getMonitors(self):
    return self.monitors
    
  # return the execution time of the current task
  # in our case task does not need to be complete since exec time is simulated
  def getCurrExecTime(self):
    return self.exectime  
