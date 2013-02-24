import unittest
import logging
import logging.config

from plot import Plot

import util.utils as Utils

# Unit tests for the program modules
class Test(unittest.TestCase):

    # Data setup
    def setUp(self):
        self.schedulefile = "../output/schedule1.yaml"
        self.execfile = "../output/executions1.yaml"
        self.schedule = Utils.loadYaml(self.schedulefile)
        self.executions = Utils.loadYaml(self.execfile)

    # Data teardown
    def tearDown(self):pass

    # Ensure loading workload from xml works
    def testPlotSchedule(self):
        print "testPlotSchedule():"
        p = Plot(self.schedule, self.executions, 80, 5, "Workoad 1")
        p.addScheduleBar()
        p.addTaskBars()
        p.addLegend()
        p.show()


# Main starts here
if __name__ == '__main__':
    logging.config.fileConfig("../conf/debug.conf")
    unittest.main()


