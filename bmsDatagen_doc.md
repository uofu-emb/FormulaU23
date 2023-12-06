# BMS Communication over CAN

We have designed some test code for an embedded system to connect into the CAN bus and act like the BMS used by the team. The BMS by Orion transmits data on an interval of 8ms. We designed our code to send 8 bytes of data that can be changed either dynamically or send static data to be interpreted and stored as battery info on the data log. This can be used to test the ability of the data logger in obtaining and storing data from the BMS.

## Intended organization of bytes

Bytes 0-3: DTC codes
Byte 4: Battery percentage (in 0.5% increments)
Byte 5: ID of cell with highest voltage
Byte 6: ID of cell with lowest voltage
Byte 7: Adaptive state of charge (compared to programmed values)

## Future use

This library can be changed in the future to use real DTC codes from the BMS documentation to update the data in the CAN frame using static definitions. At the current time, the data is static and does not reflect an updating condition inside a battery unit. It is useful only to assess the ability of the data logger to take BMS data from the CAN.

This library should be used within a thread on the system you are using to communicate on CAN, it is just a sender designed to represent the BMS sending information periodically. We recommend just testing it in loopback mode. That way this library can be used in conjunction with other libraries for testing.
