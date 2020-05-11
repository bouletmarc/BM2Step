# BM2Step
Arduino based 2Step Launch Control Module similar to Bee*R for Honda/Acura

# Compatibility

-All Honda/Acura engines that are 4cylinder and running with a distributor (mainly SerieB, D, F and H engines).
-It does works on Automatics cars.

# Installation

Inputs wires:
- ICM - Ignition Control Module signal from the ECU
- Potentiometer - Launch RPM selection
- Potentiometer - Cut RPM selection
- Activate Launch/2Step (sort to ground to activate)

Outputs wires:
- ICM - Ignition Control Module singal Out for the distributor

# How to use the BM2Step

1. Choose your desired Launch Control 2Step RPM with the potentiometer
2. Choose your desired Launch Control 2Step Cutting rpm windows with the potentiometer
3. Activate the Launch/2Step by sorting the Activate 'input' to the ground
4. Bring the engine RPM to the desired Launch Control 2Step RPM
5. Enjoy!

# How the BM2Step works

The BM2Step works by disabling the ICM ouputs when the BM2Step are active and the engine rpm reach the Set RPM. The diagram have 2x 2n4401 transistors to control the direction of the ICM outputs. One transistor are sending the ICM signal back to the distributor (sending to the ICM ouputs on the BM2Step) while the other transistor are sending the ICM signal in a 'built-in distributor simulator'.
