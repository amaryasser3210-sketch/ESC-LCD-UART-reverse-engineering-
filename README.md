# The complete approach

When trying to connect through i2c. At first I got no response at all.
I noticed that when connecting the VDD and GND with the MCU the warning starts blinking hopefully that indicates that a Slave mode waiting for master commands from the MCU.
Tried to send shake hand massage and wait for respond from it but **no response**.
The SDA and SCL is always  pulled down (0v) through the Screen (Means it refuses any external communication). I got confused after I turned off the controller and powered the screen from the Arduino It reads 0.7v not 0v. 

<img width="400" height="500" alt="LCD i2c port" src="https://github.com/user-attachments/assets/82588058-5019-4b81-ba79-8954a5e68e05" />

Then I tried to read the voltage on the **screen wire signal** (White wire) it varies between 
0.7~2.2v -> Tx (UART)
Connected the pin 10 as Rx (just listening) with the signal (the white) wire and common the ground with the screen board. and then tried to figure out the baud rate of the signal (found that it's 1200 later) and then the hard part is figuring out the data frame structure and getting data from it.

The lcd pcb basic connection is through 3 wires: (48+ve)red, (0-ve)black, (+3.3v UART signal)white.
<img width="400" height="500" alt="IMG_4962" src="https://github.com/user-attachments/assets/5c2f7fdb-43f5-42db-a6b0-69e217be81ee" />

I tried manually changing some configurations and watch the bytes change. like the gears, check engine indicator,  and then realized that the frame size is **49 Byte** and has a consistent footer 0x00 fixed. also the values of all bytes changes between fixed values like 0x08, 0xE8, 0x0f, 0xEF. that fixed readings is a good news.  
<img width="400" height="300" alt="image" src="https://github.com/user-attachments/assets/303a110d-37c0-4793-b4e8-507928c48995" />

After many readings aquishing i noticed that
byte[19] is for gears: 0x08 G1 | 0xE8 G2 | 0x0F G3
byte[18] i think indicates when motor slows down: 0xE8 None | 0xEF Decelerating
byte[16] is for Ready:  0x08 ON | 0xE8 OFF
byte[12] is for check engine:  0xE8 ON | 0x08 OFF
byte[47] i think is a checksum
