from time import sleep

from Servo import ServoController
from DataReceiver import DataReceiver


class Printer:
    def operate(self, data):
        if len(data) != 2:
            print("Invalid data length.")
            return

        servoIndex = data[0]
        angle = data[1]

        print(f"Received command: Servo {servoIndex}, Angle {angle}°")

if __name__ == "__main__":
    while True:
        try:
            # printer = Printer()
            servoController = ServoController()
            dataReceiver = DataReceiver(servoController)
            dataReceiver.start()

        except KeyboardInterrupt:
            print("\nInterrupted.")
        finally:
            servoController.disable()
        sleep(1)
