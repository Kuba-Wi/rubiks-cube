PWM1_PATH = "/sys/class/pwm/pwmchip7/pwm-7:0"
PWM2_PATH = "/sys/class/pwm/pwmchip7/pwm-7:1"

PERIOD_NS = 20_000_000      # 20 ms = 50 Hz
MIN_US = 1000               # 0°
MAX_US = 2000               # 180°

class Servo:
    def __init__(self, pwm_path):
        self.pwm_path = pwm_path
        self._write("period", PERIOD_NS)
        self._write("enable", 1)

    def _write(self, name, value):
        with open(f"{self.pwm_path}/{name}", "w") as f:
            f.write(str(value))

    def set_pulse(self, us):
        us = max(MIN_US, min(MAX_US, us))
        self._write("duty_cycle", us * 1000)

    def set_angle(self, angle):
        """Sets an angle 0-180°."""
        angle = max(0, min(180, angle))

        pulse = MIN_US + (MAX_US - MIN_US) * angle / 180
        self.set_pulse(int(pulse))

    def disable(self):
        self._write("enable", 0)


class ServoController:
    def __init__(self):
        self.servo1 = Servo(PWM1_PATH)
        self.servo2 = Servo(PWM2_PATH)

    def operate(self, data):
        if len(data) != 2:
            print("Invalid data length.")
            return

        servoIndex = data[0]
        angle = data[1]

        if servoIndex == 0:
            print(f"Setting Servo 0 to {angle}°")
            self.servo1.set_angle(angle)
        elif servoIndex == 1:
            print(f"Setting Servo 1 to {angle}°")
            self.servo2.set_angle(angle)
        else:
            print(f"Invalid servo index: {servoIndex}")

    def disable(self):
        self.servo1.disable()
        self.servo2.disable()
