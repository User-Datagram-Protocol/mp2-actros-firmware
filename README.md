A real-time CLI sim/firmware modular program of a Mercades-Benz Actros MP2. This models core vehicle systems including engine dynamics, transmission logic, pneumatic air systems, and ABS all in C.

main.c :Entry point and central sim loop.
ecu.c :Handles RPM, throttle, oil and coolent temps and boost dynamics.
air.c :Manages air pressure, dryer status, and pneumatic subsystems.
tcm.c :Implements transmission logic and gear shifting behavior.
abs.c :Simulates wheel speed and braking system status.

Build instructions:
git clone https://github.com/User-Datagram-Protocol/mp2-actros-firmware.git
cd actros
gcc main.c air.c tcm.c abs.c ecu.c -o actros

controls
w Increse throttle
s Decrease throttle
q Shutdown system and Exit
