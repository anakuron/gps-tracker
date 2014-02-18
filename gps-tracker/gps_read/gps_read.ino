#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <string>

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

static const int RXPin = 5, TXPin = 3;
static const int SD_SS = 4;
static const int led = 13;
static const uint32_t GPSBaud = 9600;
//unsigned long time; //Will overflow in 50 days!!

File myFile;

void setup_sd()
{
 // Open serial communications and wait for port to open:
  //Serial.begin(9600);
   //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  if (!SD.begin(SD_SS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  
  // re-open the file for reading:
  /*
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  */
}

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

char * filename;
int filename_initialized = 0;

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  pinMode(led, OUTPUT);
  setup_sd();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      displayInfo();
      //Serial.println(gps.time.value());
        if(gps.time.isValid()) {
          //digitalWrite(led, HIGH);
          if(gps.time.isValid() && gps.time.value() != 0) {
              digitalWrite(led, HIGH);
          } else {
             digitalWrite(led, LOW);
          }
        Serial.println("Time is valid and not zero.");
        //Serial.println("test_file.log"_initialized);
        String temp_fn;
        if(!filename_initialized) { // "test_file.log" needs to be initialized
          Serial.println("In the initialization loop");
          //temp_fn = get_filename_string(gps);
          //Serial.println(temp_fn);
          //filename = (char *)malloc(sizeof(char)*(temp_fn.length() + 1));
          //temp_fn.toCharArray(filename,256);
          //filename_initialized = 1;
        }
        //Serial.println(temp_fn);
      myFile = SD.open("test.log", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to ");
    //myFile.println("testing 1, 2, 3.");
    write_to_sd();
    //myFile.println(displayInfo());
    // close the file:
    Serial.println("done.");
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening ");
  }
  //read_from_sd();
        }
    }
    if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
    }
  }
}

void read_from_sd() {
   myFile = SD.open("test.log");
  if (myFile) {
          Serial.println(":");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening ");
  } 
}

void write_to_sd() {
    myFile.print(F("Location: "));
  if (gps.location.isValid())
  {
    myFile.print(gps.location.lat(), 6);
    myFile.print(F(","));
    myFile.print(gps.location.lng(), 6);
  }
  else
  {
    myFile.print(F("INVALID"));
  }

  myFile.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    myFile.print(gps.date.month());
    myFile.print(F("/"));
    myFile.print(gps.date.day());
    myFile.print(F("/"));
    myFile.print(gps.date.year());
  }
  else
  {
    myFile.print(F("INVALID"));
  }

  myFile.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) myFile.print(F("0"));
    myFile.print(gps.time.hour());
    myFile.print(F(":"));
    if (gps.time.minute() < 10) myFile.print(F("0"));
    myFile.print(gps.time.minute());
    myFile.print(F(":"));
    if (gps.time.second() < 10) myFile.print(F("0"));
    myFile.print(gps.time.second());
    myFile.print(F("."));
    if (gps.time.centisecond() < 10) myFile.print(F("0"));
    myFile.print(gps.time.centisecond());
  }
  else
  {
    myFile.print(F("INVALID"));
  }

  myFile.println();
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

String get_filename_string(TinyGPSPlus gps) {
  return "df.log";
}
