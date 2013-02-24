import logging

import util.utils as Utils
from jobqueue import JobQueue

class Scheduler(object):

  # initialize scheduler 
  def __init__(self, workload, processor):

    # create logger
    self.log = logging.getLogger("Scheduler")

    self.processor = processor
    self.workload = workload
 
    # create set of periods for calculate gcd and lcm
    periodlist = []
    for id,task in workload.iteritems():
      task['minstart'] = 0
      periodlist.append(task['period'])
    self.periods = set(periodlist)

    self.jobq = JobQueue()
    self.cycle = 0

    # interrupts to dispatch jobs happen at gcd of all periods
    self.interrupt = Utils.gcdd(*self.periods)

    # hyperperiod is lcm of all periods
    self.H = Utils.lcmm(*self.periods)

    # schedule output dictionary
    self.schedule = []

  # run the workload
  def run(self):
    prev_etask = None
    prev_id = None
    for i in range(self.H):
      # dispatcher runs at interrupt frequency
      if i % self.interrupt == 0:
        for id,task in self.workload.iteritems():
          # make a copy of the task so each one can have different values written to it
          qtask = task.copy()
          period = qtask['period']
          if i % period == 0:
            # need to track the earliest possible start of task so it doesn't execute before offset has expired
            qtask['minstart'] = i + qtask['offset']
            qtask['id'] = id
            self.log.debug("Time:" + str(i) + " Dispatching task:" + str(task))
            self.jobq.insertTask(qtask)

      # executor runs for each i to simulate a separate thread
      if self.jobq.length() > 0:
        # if processor is not executing something
        if not self.processor.busy():
          # just pop item from front of queue
          task = self.jobq.popHighestPriorityJob()
          if task != None:
            # check if offset has expired, if not we can't execute anything
            if i >= task['minstart']: 
              self.log.debug("Time:" + str(i) + " Executing task:" + str(task))
              # pass in random time strategy for simulating execution time
              self.processor.execute(task, Utils.getRandomET)
              # add task to schedule
              del task['minstart']
              task['start'] = i
              task['et'] = self.processor.getCurrExecTime()
              self.schedule.append(task)
            else:
              self.log.debug("Time:" + str(i) + " Offset not expired, idling")
              # insert task back at front of queue
              self.jobq.insertTaskAtFront(task)

      # tell processor time has moved forward
      self.processor.incrTime()

    self.log.debug(str(self.schedule))
    for monitor in self.processor.getMonitors():
      self.log.debug(str(monitor.getResults()))

  # get schedule so we can do stuff with it
  def getSchedule(self):
    return self.schedule

  # get hyperperiod
  def getH(self):
    return self.H
