# BM2Step
Arduino based 2Step Launch Control Module similar to Bee*R for Honda/Acura

# Compatibility

- All Honda/Acura engines that are 4cylinder and running with a distributor (mainly SerieB, D, F and H engines).
- It does works on Automatics cars.

# Parts List
- 1x Arduino Nano V3 5V 16mhz atmega328p
- 1x 74HC00 Quad 2-input NAND gate (or 74LVC1G00 for a Single gate rather than a Quad gate)
- 2x 2N4401 transistors
- 6x 1K Ohms resistors
- 1x 1 to 10 Ohm resistors (can be replaced with a jumper or '0ohm' directly but a resistor make the circuit safer!)
- 2x B10K Potentiometer 0931
- 2x 2mm to 5mm Leds
- 1x 7805/78M05 5v voltage regulator
- 1x 100nF capacitor (104)
- 1x Diode (minimum of 20v), can be 1N4002, 1N4004, 1N4148, etc..

# Installation

Inputs wires:
- ICM - Ignition Control Module signal from the ECU
- Potentiometer - Launch RPM selection
- Potentiometer - Cut RPM selection
- Activate Launch/2Step (sort to ground to activate)
- 12v Ignition On
- Ground

Outputs wires:
- ICM - Ignition Control Module singal Out for the distributor

**You don't need to power the arduino from the USB, the 7805/78M05 5v regulator output 5v from the 12v Ignition input wire to power the arduino when the Car ignition key are turned On!**

# How to use the BM2Step

1. Choose your desired Launch Control 2Step RPM with the potentiometer
2. Choose your desired Launch Control 2Step Cutting rpm windows with the potentiometer
3. Activate the Launch/2Step by sorting the Activate 'input' to the ground
4. Bring the engine RPM to the desired Launch Control 2Step RPM
5. Enjoy!

![alt tag](https://github.com/bouletmarc/BM2Step/blob/master/Schematic/eagle_2020-05-11_03-45-24.png)

# How the BM2Step works

The BM2Step works by disabling the ICM ouputs when the BM2Step are active and the engine rpm reach the Set RPM. The diagram have 2x 2n4401 transistors to control the direction of the ICM outputs. One transistor are sending the ICM signal back to the distributor (sending to the ICM ouputs on the BM2Step) while the other transistor are sending the ICM signal in a 'built-in distributor simulator'.
