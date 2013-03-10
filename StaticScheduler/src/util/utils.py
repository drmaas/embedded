import yaml
import random

# some common utils
# gcd, lcm from http://www.4dsolutions.net/cgi-bin/py2html.cgi?script=/ocn/python/primes.py

def gcd(a, b):
  """Return greatest common divisor using Euclid's Algorithm."""
  while b:
    a, b = b, a % b
  return a

def gcdd(*args):
  """Return gcd of args."""   
  return reduce(gcd, args)

def lcm(a, b):
  """Return lowest common multiple."""
  return a * b // gcd(a, b)

def lcmm(*args):
  """Return lcm of args."""   
  return reduce(lcm, args)

# load yaml from file and return as a dictionary
def loadYaml(filename):
  file = open(filename)
  content = yaml.safe_load(file)
  file.close()
  return content

# dump yaml to file
def dumpYaml(content, filename):
  file = open(filename, 'w')
  yaml.dump(content, file)
  file.close()

# dump to file
def dumpSchedule(schedule, filename):
  # print schedule vertically
  file = open(filename, 'w')
  for job in schedule:
    file.write("Start Time:"+str(job['start'])+" Task Id:"+str(job['id'])+" Expected Duration:"+str(job['wcet'])+"\n")

  file.close()

# deep copy of workload
def deepcopy(workload):
  newworkload = {}
  for id,task in workload.iteritems():
    newworkload[id] = task.copy()
  return newworkload

# just return wcet, random time not needed
def getRandomET(wcet):
  #return random.randrange(1, wcet+1, 1)
  return wcet
