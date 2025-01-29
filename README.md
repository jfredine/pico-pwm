# pico-pwm

A simple example of generating a PWM signal on the Raspberry Pi Pico using the
SDK.  The user inputs a value representing the high time (in micro seconds)
of the pwm signal.  This value should be between 800 and 2200.  Upon receiving
a new value in this range the PWM signal is reprogrammed with the new high time.
If a servo is connected to this signal it should cause the servo arm to move.