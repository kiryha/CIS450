# CIS450
This program uses mutex and conditional variables.  Whenever the count for the number
of cars on the bridge is modified, it is locked to ensure mutual exclusivity.  The conditional
variable &safe is used to give cars a programming "green light" to cross the bridge.  The algorithm
supplied along with this implementation	ensures that no collisions or bridge capacity violations
will occur between the 50 cars trying to cross the bridge.

The program simulates 50 cars crossing the bridge in different directions (randomly). 
Program avoids car to pass the bridge if:
- number of cars exceeds the bridge capacity of 3 cars
- cars moving the opposite direction at the same time
The mutex (to keep track of numbers of cars) and conditional variables (to allow car to pass the bridge) used 
to build required functionality. 