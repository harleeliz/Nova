import pygame
import serial
import time
import serial.tools.list_ports

# List available serial ports
ports = serial.tools.list_ports.comports()
for port in ports:
    print(port.device)

# ----- CONFIGURE BLUETOOTH PORT -----
BLUETOOTH_PORT = "/dev/rfcomm0"  # Update if necessary
BAUD_RATE = 9600

print("Connecting to pi pico Bluetooth...")
bt = serial.Serial('/dev/tty.PicoWSerial28CDC10B1175', BAUD_RATE)
time.sleep(2)
print("Connected to robot!")

pygame.init()
print("Keyboard control mode activated.")

# ----- SEND COMMAND FUNCTION -----
def send_command(cmd):
    print(f"Sending: {cmd}")
    bt.write(cmd.encode())

# ----- KEYBOARD MOVEMENT LOGIC -----
def get_keyboard_command(x, y, deadzone=0.3):
    if abs(x) < deadzone and abs(y) < deadzone:
        return 'q'  # Stop Motors only

    if y < -deadzone and abs(x) < deadzone:
        return 'w'  # Forward
    elif y > deadzone and abs(x) < deadzone:
        return 's'  # Backward
    elif x < -deadzone:
        return 'l'  # Turn Left
    elif x > deadzone:
        return 'r'  # Turn Right
    return 'q'  # Safety Stop

# ----- MAIN LOOP -----
last_cmd = ''

try:
    print("Control with: W/A/S/D (Move) | L/R (Turn)")
    print("Use 'e' and 'f' keys to manually enter speeds.")

    while True:
        pygame.event.pump()

        keys = pygame.key.get_pressed()

        # Handle movement keys
        x_axis = 0
        y_axis = 0

        if keys[pygame.K_w]:
            y_axis = -1
        elif keys[pygame.K_s]:
            y_axis = 1
        if keys[pygame.K_a]:
            x_axis = -1
        elif keys[pygame.K_d]:
            x_axis = 1

        command = get_keyboard_command(x_axis, y_axis)

        # Only send command if changed
        if command != last_cmd:
            send_command(command)
            last_cmd = command

        # Motor Speed Change (Type e + Enter)
        if keys[pygame.K_e]:
            speed = input("Enter Motor Speed (0-255): ")
            send_command(f"E {speed}")

        # Fan Speed Change (Type f + Enter)
        if keys[pygame.K_f]:
            fanspeed = input("Enter Fan Speed (0-255): ")
            send_command(f"F {fanspeed}")

        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")
    bt.close()