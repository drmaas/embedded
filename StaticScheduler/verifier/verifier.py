import logging

class Verifier(object):

  def __init__(self, ex, H, name):
    
    self.log = logging.getLogger("Verifier")
    
    self.ex = ex
    self.H = H
    self.name = name

  def verifySchedule(self):
    
    # verify tasks meeting requirements
    self.log.info("###################################################")
    self.log.info("Verifying timing requirements for " + self.name + " tasks")
    self.log.info("###################################################")
    for id,task in self.ex.iteritems():
      self.verifyTask(id,task)

    # verify tasks don't overlap
    self.log.info("")
    self.log.info("Verifying that only one task runs at a time for the entire hyperperiod")
    self.log.info("")
    self.verifySingleThread(self.ex)
     
  # verify task runs each period between offset and ends by deadline
  def verifyTask(self, id, task):
    self.log.info("")
    self.log.info("Verifying timing requirements for task " + id)
    self.log.info("")
    period = task['period']
    offset = task['offset']
    deadline = task['deadline']
    times = task.get('times', [])

    # check each period iterrupt
    run = False
    verified = False
    for i in range(0, self.H, period):
      minstart = i+offset
      maxend = i+offset+deadline
      for time in times:
        start = time['start']
        end = time['start'] + time['duration']

        # did the task run?
        if start >= i and end <= i+period:
          run = True
          # did it meet timing?
          if start >= minstart and end <= maxend:
            self.log.info("Success for task " + id + " at period interrupt " + str(i))
            verified = True
            break
          else:
            self.log.info("Timing failure for task " + id + " during period interrupt " + str(i) + ". Oops.")
            self.log.info("Minimum start:" + str(minstart) + " Actual start:"+str(start))
            self.log.info("Maximum end:" + str(maxend) + " Actual end:"+str(end))

      if not run:
        self.log.info("Task " + id + " did no run during period interrupt " + str(i) + ". Oops.")

  # verify no two tasks ever overlap
  def verifySingleThread(self, tasks):
    success = True
    for i in range(self.H):
      runningtasks = {}
      for id,task in tasks.iteritems():
        times = task.get('times', [])
        for time in times:
          start = time['start']
          end = start + time['duration']
          # running is defined to include start, not include end
          if i >= start and i < end:
            runningtasks[id] = task
      if len(runningtasks) > 1:
        success = False
        self.log.info("")
        self.log.info("Failure: multiple tasks are running at time " + str(i))
        for id,task in runningtasks.iteritems():
          self.log.info("Running:"+id)
    if success:
      self.log.info("Verified that only one task runs at a time.")
    else:
      self.log.info("Check for one process running at a time failed.")
      
