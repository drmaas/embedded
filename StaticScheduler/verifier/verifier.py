class Verifier(object):

  def __init__(self, ex, H):
    self.ex = ex
    self.H = H

  def verifySchedule(self):
    # verify tasks meeting requirements
    print
    print "Verifying timing requirements for all tasks"
    print
    for id,task in self.ex.iteritems():
      self.verifyTask(id,task)

    # verify tasks don't overlap
    print
    print "Verifying that only one task runs at a time for the entire hyperperiod"
    print
    self.verifySingleThread(self.ex)
     
  # verify task runs each period between offset and ends by deadline
  def verifyTask(self, id, task):
    print
    print "Verifying timing requirements for task " + id
    print
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
            print "Success for task " + id + " at period interrupt " + str(i)
            verified = True
            break
          else:
            print "Timing failure for task " + id + " during period interrupt " + str(i) + ". Oops."
            print "Minimum start:" + str(minstart) + " Actual start:"+str(start)
            print "Maximum end:" + str(maxend) + " Actual end:"+str(end)

      if not run:
        print "Task " + id + " did no run during period interrupt " + str(i) + ". Oops." 

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
	print
        print "Failure: multiple tasks are running at time " + str(i)
        for id,task in runningtasks.iteritems():
          print "Running:"+id
    if success:
      print "Verified that only one task runs at a time."
    else:
      print "Check for one process running at a time failed."
      
