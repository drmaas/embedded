import util.utils as Utils

# class to create results of an execution
class ProcessorMonitor(object):

  def __init__(self, workload):
    self.workload = workload
    self.results = Utils.deepcopy(self.workload)

  # add a job execution result to the task structure
  def addExecution(self, tid, time):
    task = self.results[tid]
    times = task.get('times', [])
    times.append(time)
    task['times'] = times

  # get executions so we can do stuff with them
  def getResults(self):
    return self.results
