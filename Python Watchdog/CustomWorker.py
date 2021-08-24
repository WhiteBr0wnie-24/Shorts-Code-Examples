import time
from threading import Thread


class CustomWorker(Thread):

    def __init__(self, startValue):
        super().__init__()
        self.stopped = False
        self.counter = startValue

    def run(self):
        while self.counter > 0 and not self.stopped:
            print(self.name + ": Remaining tasks: " + str(self.counter))
            self.counter -= 1
            time.sleep(0.5)
