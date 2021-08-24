# Simple Python watchdog that detects if a thread stopped (e.g., due to an error)
# and restarts the thread if necessary.

import time
from CustomWorker import CustomWorker

# Main entry point
# Start the watchdog here
if __name__ == '__main__':
    # The thread this watchdog controls
    t = False
    try:
        # Run the watchdog endlessly
        while True:
            # If t is False then the thread was either stopped or never started at all
            # Therefore, create a new Instance and assign it to t, then start the thread
            if not t:
                t = CustomWorker(5)
                t.start()
                print("Started the thread!")
            # Check whether t stopped
            if t and not t.is_alive():
                print("Thread is not running!")
                print("Restarting the thread...")
                t = False
            # If t is running, just wait and let the other threads work
            else:
                time.sleep(1.0)
    # Users can exit the watchdog by sending a keyboard interrupt (Ctrl + C)
    except KeyboardInterrupt:
        print("Stopping all worker threads...")
        wait_cycles = 0

        # If t is currently running, send a stop signal and wait for the
        # thread to finish.
        if t and t.is_alive():
            t.stopped = True

        # Make sure that your custom threads don't block, otherwise the
        # watchdog will never exit
        while t.is_alive():
            print("Waiting for a worker to stop...")
            time.sleep(0.5)

        print("Stopped all workers! Stopping the watchdog...")
