import logging

# class to store a job queue and provide method to get highest priority (smallest relative deadline)
class JobQueue(object):

  def __init__(self):
    # create logger
    self.log = logging.getLogger("JobQueue")

    self.q = []

  # insert a task at the right place
  def insertTask(self, task):
    inserted = False
    for n in self.q:
      task_deadline = task['deadline']
      n_deadline = n['deadline']
      if task_deadline < n_deadline:
        index = self.q.index(n)
        self.q.insert(index, task)
        self.log.debug("Inserted task at " + str(index) + ": " + str(task))
        inserted = True
        break
    if not inserted:
      self.q.append(task)
      self.log.debug("Inserted task at " + str(len(self.q)) + ": " + str(task))

  # highest priority always on the left
  def popHighestPriorityJob(self):
    if len(self.q) > 0:
      return self.q.pop(0)
    else:
      return None

  # put task at front of q
  def insertTaskAtFront(self, task):
    self.q.insert(0, task)

  # length
  def length(self):
    return len(self.q)    
