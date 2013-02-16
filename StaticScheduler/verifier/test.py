import unittest
import logging
import logging.config

from verifier import Verifier
import util.utils as Utils

# Unit tests for the program modules
class Test(unittest.TestCase):

  # Data setup
  def setUp(self):
    self.e1 = "../output/executions1.yaml"
    self.e2 = "../output/executions2.yaml"

  # Data teardown
  def tearDown(self):pass

  # Test verifier
  def testVerifier(self):
    print "testVerifier():"
    v = Verifier(Utils.loadYaml(self.e1))
    v.verifySchedule() 
    print 

# Main starts here
if __name__ == '__main__':
  logging.config.fileConfig("../conf/debug.conf")
  unittest.main()
