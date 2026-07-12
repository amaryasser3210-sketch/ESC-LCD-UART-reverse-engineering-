#define RX_PIN 16
#define TX_PIN 17
#define FRAME_SIZ 50

int i = 0;
uint8_t b[FRAME_SIZ];
uint8_t idle[FRAME_SIZ] = {
  // This is the idle frame values, store it to show the changed only bytes
  // Zero for the non intersting values
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  // Indices 30 - 49 (your speed specified values)
  0xE8, 0x08, 0xEF, 0x08, 0xE8, 0x08, 0xEF, 0x08, 0xE8, 0x08, // 30 - 39
  0xEF, 0x08, 0xE8, 0x08, 0xEF, 0x08, 0x08, 0x0F, 0xE8, 0x00  // 40 - 49
};

// --------------------------------------------
// Takes the frame and decode the values from fixed bytes
// byte[19] is for gears: 0x08 G1 | 0xE8 G2 | 0x0F G3
// byte[18] i think indicates when motor slows down: 0xE8 None | 0xEF Decelerating
// byte[16] is for Ready: 0x08 ON | 0xE8 OFF
// byte[12] is for check engine: 0xE8 ON | 0x08 OFF
// byte[47] i think is a checksum
// --------------------------------------------

// this function prints the state of the data i had known   
void printStatus(uint8_t b[]) {
  Serial.println();
  Serial.print("Check engine is :");
  Serial.println((uint8_t)(b[12] == 0x08) ? "OFF" : "ON");
  Serial.print("Ready LED is :");
  Serial.println((uint8_t)(b[16] == 0xE8) ? "OFF" : "ON");
  Serial.print("Descelerating :");
  Serial.println((uint8_t)(b[18] == 0xEF) ? " active" : " non active");
  Serial.print("Gear is :");
  if ((uint8_t)b[19] == 0x08)
    Serial.print(" 1");
  else if ((uint8_t)b[19] == 0xE8)
    Serial.print(" 2");
  else if ((uint8_t)b[19] == 0x0F)
    Serial.print(" 3");
  else
    Serial.print("Gear Error!");
  Serial.println();
}

// print the known parameters values of the array in HEX
void printValues(uint8_t b[]) {
  Serial.println();
  Serial.print("Check engine is :");
  Serial.println(b[12], HEX);
  Serial.print("Ready LED is :");
  Serial.println(b[16], HEX);
  Serial.print("Descelerating :");
  Serial.println(b[18], HEX);
  Serial.print("Gear is :");
  Serial.print(b[20], HEX);
  Serial.println();
}

// Prints the values recieved on a HardwareSerial and prints it in Frame format  
void printFrames(Stream &Serial1) {
  if (Serial1.available()) {
    Serial.print(i++);
    Serial.print(":");
    b[i - 1] = Serial1.read();
    if (b[i - 1] < 0x10) Serial.print('0');
    Serial.print(b[i - 1], HEX);
    Serial.print(" ");
    if (b[i - 1] == 0x00) {
      // printValues(b);
      // printStatus(b);
      Serial.println();
      i = 0;
    }
  }
}

// Prints the values recieved on a HardwareSerial and prints it in Frame format starting from "start"
void printFrames(Stream &Serial1, int start) {
  if(Serial1.available())
  {
    b[i] = Serial1.read();
    if (i > start) {
      Serial.print(i);
      Serial.print(":");
      if (b[i] < 0x10) Serial.print('0');
      Serial.print(b[i], HEX);
      Serial.print(" ");
    }
    if (b[i] == 0x00) {
      Serial.println();
      i = 0;
    }
    i++;
    if (i > 49) {
      Serial.println("\n[Error] Buffer overflow! Resetting frame.");
      i = 0;
    }
  }
}


// Prints the values recieved on a HardwareSerial and prints it in Frame format starting from "start" and shows only the different bytes from the idle frame depending on the "diff"
void printFrames(Stream &Serial1, int start,bool diff) {
  if(Serial1.available())
  {
    b[i] = Serial1.read();
    if (i > start) {
      Serial.print(i);
      Serial.print(":");
      Serial.print(b[i]-idle[i]);
      Serial.print(" ");
    }
    if (b[i] == 0x00) {
      Serial.println();
      i = 0;
    }
    i++;
    if (i > 49) {
      Serial.println("\n[Error] Buffer overflow! Resetting frame.");
      i = 0;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(1200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void loop() {
  printFrames(Serial1,30);
}
