#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
//#include <string>

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
   
  while(!SD.begin(SD_SS)) {
    Serial.println("initialization failed!... waiting until SD card is inserted...");
    delay(5000);
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

char filename[41]; // 8/8/8/8.3\0
int filename_initialized = 0;
unsigned long start_time;
boolean write_gpx = true;
void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);

//  Serial.println(F("DeviceExample.ino"));
//  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
//  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
//  Serial.println(F("by Mikal Hart"));
//  Serial.println();
  pinMode(led, OUTPUT);
  setup_sd();
  start_time = millis();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      displayInfo();
      //Serial.println(gps.date.value());
        if(gps.date.isValid() && gps.date.value() != 0) {
          //jee
          //digitalWrite(led, HIGH);
          //if(gps.time.isValid() && gps.time.value() != 0) {
          //    digitalWrite(led, HIGH);
          //} else {
          //   digitalWrite(led, LOW);
          //}
        Serial.println("Time is valid and not zero.");
        //Serial.println("test_file.log"_initialized);
        //String temp_fn;
        if(!filename_initialized) { // "test_file.log" needs to be initialized
          Serial.println("In the initialization loop");
          //temp_fn = get_filename_string(gps);
          //Serial.println(temp_fn);
          //filename = (char *)malloc(sizeof(char)*(temp_fn.length() + 1));
          //temp_fn.toCharArray(filename,256);
          set_filename(gps,filename,40);
          filename_initialized = 1;
          
          if(write_gpx) {
            myFile = SD.open(filename, FILE_WRITE);
            if(myFile) {
           start_gpx_file(); 
           start_gpx_track();
           start_gpx_segment();
           myFile.close();
            } else {
              Serial.println("Failed to open the file for writing the header.");
              return;
            }
          }
          
          Serial.println("Exiting form the initialization loop");
        }
        //Serial.println(temp_fn);
        if(gps.location.isValid()) {
           if(true || gps.location.isUpdated()) {
             //Serial.println(filename);
         myFile = SD.open(filename, FILE_WRITE);
  // if the file opened okay, write to it:
          if (myFile) {
            Serial.print("Writing to file.");
    //myFile.println("testing 1, 2, 3.");
            if(write_gpx) {
              write_track_point(gps.location.lat(),gps.location.lng(),gps.altitude.meters(),gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour(),gps.time.minute(),gps.time.second());
            } else {
              write_to_sd();
            }
    //myFile.println(displayInfo());
    // close the file:
            Serial.println("done.");
            myFile.close();
        } else {
    // if the file didn't open, print an error:
          Serial.println("error opening ");
        }
           } else {
            Serial.println("Coordinates need not be updated."); 
           }
        } else {
         Serial.println("Location not valid");
         //Serial.println("WTF");
        }
        
  //read_from_sd();
        }
    }
    if (millis() - start_time > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
    }
  }
}

void set_filename(TinyGPSPlus gps, char * const buffer,int length) {
  String filename = "";
  filename = filename + gps.time.value() + ".gpx";
  Serial.println(filename);
  //filename = filename + gps.date.year();
  //char dir_name[28];
  //filename.toCharArray(dir_name,27);
  //Serial.println(filename);
  //Serial.println(dir_name);
  //if(!SD.exists(dir_name)) {
    //Serial.println("directory " + filename + " not found. Creating.");
    //SD.mkdir(dir_name);
  //}
  //Serial.println(filename);
  //Serial.println(dir_name);
  
  //filename = filename + "/" + gps.date.month();
  //Serial.println(filename);
  //Serial.println(dir_name);
  //filename.toCharArray(dir_name,27);
  /*if(!SD.exists(dir_name)) {
   SD.mkdir(dir_name);
  }*/
  //filename = filename + "/" + gps.date.day();
  //Serial.println(filename);
  //Serial.println(dir_name);
    //filename.toCharArray(dir_name,27);
  /*if(!SD.exists(dir_name)) {
   SD.mkdir(dir_name);
  }*/
  
  /*
  char dir_name[27];
  filename.toCharArray(dir_name,27);
  if(!SD.exists(dir_name)) {
    SD.mkdir(dir_name);
  }
  */
  //int mkdir_success = SD.mkdir(dir_name);
  //Serial.print("Successfully making a directory: ");
  //Serial.println(mkdir_success);
  //Serial.println(filename);
  //filename = filename + "/";
  //Serial.println(filename);
  //filename = filename + gps.time.value();
  //Serial.println(filename);
  //Serial.println(filename);
  //filename = filename + ".DAT";
  //Serial.println(filename);
  //filename = filename + "/" + gps.time.value() + ".DAT";
  //filename = filename + "/" + gps.time.value();
  //Serial.println(filename.length());
  //if(filename.length() > length) {
   //filename = "temp";// Error! 
  //}
  //Serial.println(filename);
  filename.toCharArray(buffer,length);
}

void read_from_sd() {
   myFile = SD.open(filename);
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
    myFile.print(F("LAT:"));
    myFile.print(gps.location.lat(), 6);
    myFile.print(F(",LNG:"));
    myFile.print(gps.location.lng(), 6);
    myFile.print(F(",ELE:"));
    myFile.print(gps.altitude.meters(),6);
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



void start_gpx_file() {
  myFile.println(F("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"));
  myFile.println(F("<gpx version=\"0.6\" creator=\"Nakkitracker 0.2 - nakkiservo@quakenet\">"));
}

void end_gpx_file() {
  myFile.println(F("<\\gpx>"));
}

void start_gpx_track() {
  myFile.println(F("<trk>"));
}

void end_gpx_track() {
  myFile.println(F("<\trk>"));
}

void start_gpx_segment() {
  myFile.println(F("<trkseg>"));
}

void end_gpx_segment() {
  myFile.println(F("<\\trkseg>"));
}

void write_track_point(double lat,double lng,double elevation,int year,int month,int day,int hour,int min, int sec) {
  myFile.print(F("<trkpt lat=\""));
  myFile.print(lat,8);
  myFile.print(F("\" lon=\""));
  myFile.print(lng,8);
  myFile.println(F("\">"));
  myFile.print(F("<ele>"));
  myFile.print(elevation,8);
  myFile.println(F("<\\ele>"));
  myFile.print(F("<time>"));
  myFile.print(year);
  myFile.print(F("-"));
  myFile.print(month);
  myFile.print(F("-"));
  myFile.print(day);
  myFile.print(F("T"));
  myFile.print(hour);
  myFile.print(F(":"));
  myFile.print(min);
  myFile.print(F(":"));
  myFile.print(sec);
  myFile.println(F("Z<\\time>"));
  myFile.println(F("<\\trkpt>"));
}




