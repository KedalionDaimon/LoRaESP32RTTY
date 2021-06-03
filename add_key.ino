// ESP32 DEV Module
// TTGO LoRa32-OLED v.2.1.6
#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

//define the pins used by the transceiver module
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define BAND  868E6

#define SRL Serial
// HardwareSerial SRL(0);
// SoftwareSerial SRL(25,26);

#define KEYDIM 65535
// ADJUST THE ABOVE IN GENCODE.LISP!

int counter = 0;

char lit1 = ' ';
char lit2 = ' ';
char lit3 = ' ';
char lit4 = ' ';
char lit5 = ' ';
char lit6 = ' '; // used only for final space

char arr0 = 0;
char arr1 = 0;
char arr2 = 0;
char arr3 = 0;
char arr4 = 0;
char arr5 = 0;
unsigned int kcount = 0; // make it long int if you want a bigger KEYDIM

char ranthrough = 0;


// The reason for this encryption scheme is to avoid any character being
// "dependent" upon transmission upon any previously received information,
// i.e. to make it quire loss tolerant, and moreover, to make it possible
// to PRINT these codes should any need arise.
// This is NOT a very strong encryption, but if you change the code every
// day and transmit little information, it might be reasonable, depending
// on the case.

// const PROGMEM char keyarray[KEYDIM][6] = ... ADD YOUR KEY HERE



void setup() {
  // start serial port
  SRL.begin(300);
  //SRL.begin(1200, SERIAL_8N1, 16, 17); // rx, tx

  //setup LoRa transceiver module
  //SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);

  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(BAND)) {
    SRL.print(".");
    delay(500);
  }

  // LoRa.setTxPower(17); //2-17
  LoRa.setSpreadingFactor(11); // 7-12
  LoRa.setSignalBandwidth(250E3); // 125/10 (preferred) or 250/11, 125/11 good but slow
  // 10 & 125 worked fine, too.
  // 11 & 12 do not allow joining of LoRaWANs - which I want to avoid anyway.
  LoRa.setCodingRate4(6); // 4:5 ... 4:8
  LoRa.setTxPower(19); //20dB output discussion: PABOOST

  
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xEE);
  SRL.println("    LORA INITIALISED.");

  int greeting = 1;

  for (;;) {

        if (greeting == 1) {
            greeting = 0;
            LoRa.beginPacket();
            LoRa.print(F("\r\n    SYSTEM ONLINE.\r\n"));
            LoRa.endPacket();
            delay(10);
        }

        // DECODE
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
        while (LoRa.available()) {
          char ch1 = static_cast<char>(LoRa.read());
          char ch2 = static_cast<char>(LoRa.read());
          char ch3 = static_cast<char>(LoRa.read());
          char ch4 = static_cast<char>(LoRa.read());
          char ch5 = static_cast<char>(LoRa.read());
          char ch6 = static_cast<char>(LoRa.read()); // final space
          char rec = 0;

// debug: show raw receipt
//          SRL.print(ch1);
//          SRL.print(ch2);
//          SRL.print(ch3);
//          SRL.print(ch4);
//          SRL.print(ch5);
//          SRL.print(ch6);

          for (kcount = 0; kcount < KEYDIM; kcount++) {
            arr0 = keyarray[kcount][0];
            arr1 = keyarray[kcount][1];
            arr2 = keyarray[kcount][2];
            arr3 = keyarray[kcount][3];
            arr4 = keyarray[kcount][4];
            arr5 = keyarray[kcount][5];
            if ((arr1 == ch1) &&
                (arr2 == ch2) &&
                (arr3 == ch3) &&
                (arr4 == ch4) &&
                (arr5 == ch5)) {
                  rec = arr0;
                  break;
                }
          }

          if (rec == '_') {
            rec = ' ';
          } else if (rec == '{') {
            rec = '\n';
          } else if (rec == '}') {
            rec = 8; // backspace
          } else if ((rec > 96) && (rec < 123)) {
            rec = rec - 32;
          }

          if (rec == '\n') {
            SRL.print('\r');
          }
          if (rec != 0) { // Quasi Jamming protection
            SRL.print(rec);
            delay(10); // 80
          }
          if (rec == '\n') {
            delay(10); // 720 320
          }
        }}

        // ENCODE
        if (SRL.available()) {
          char j = SRL.read();

          // delete the following line if you want locally to have lowercase
          if ((j > 96) && (j < 123)) { j = j - 32; }
          
          SRL.print(j); // turn this on if localecho is needed - needed for netcat, NOT needed for telnetd
          
          if (j != '\r') {
          if (j == ' ') {
            j = '_';
          } else if (j == '\n') {
            j = '{';
          } else if (j == 8) {
            j = '}'; // backspace
          } else if ((j > 96) && (j < 123)) {
            // remotely, uppercase is enforced
            j = j - 32;
          } else if ((j < 32) || (j == 39) || (j == 123) || (j == 125) || (j == 92) || (j == 95) || (j > 126)) {
            j = 'X';
          }

          // rand() % n produces values in the range 0 - (n-1)
          kcount = rand() % KEYDIM;
          // printf("\nkcount = %ld\n", kcount);

          lit1 = ' ';
          lit2 = ' ';
          lit3 = ' ';
          lit4 = ' ';
          lit5 = ' ';
          ranthrough = 0;
          while (1) {
            if (ranthrough == 2) { break; }

            if (keyarray[kcount][0] == j) {
              lit1 = keyarray[kcount][1];
              lit2 = keyarray[kcount][2];
              lit3 = keyarray[kcount][3];
              lit4 = keyarray[kcount][4];
              lit5 = keyarray[kcount][5];
              break;
            }

            kcount++;

            if (kcount == KEYDIM) {
              kcount = 0;
              ranthrough++;
            }

          }

          LoRa.beginPacket();
          LoRa.write(static_cast<char>(lit1));
          LoRa.write(static_cast<char>(lit2));
          LoRa.write(static_cast<char>(lit3));
          LoRa.write(static_cast<char>(lit4));
          LoRa.write(static_cast<char>(lit5));
          LoRa.write(static_cast<char>(' '));
          LoRa.endPacket();

          delay(30); // needed to access the array, 15 not good enough, this works.

          //    }
        }}

  }

}

void loop() {}
