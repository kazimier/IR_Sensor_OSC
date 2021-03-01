#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <SharpDistSensor.h>



const char* ssid = "*******"; // ssid
const char* password = "********";// password
IPAddress ip(192, 168, 1, 25); //set static ip
IPAddress gateway(192, 168, 1, 1); //set getteway
IPAddress subnet(255, 255, 255, 0);//set subnet
 

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress DestIp(192,168,1,228);        // remote IP of the destination PC
const unsigned int DestPort = 9001;          // remote port 
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)


// Analog pin to which the sensor is connected
const byte sensorPin = A0;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

// Create an object instance of the SharpDistSensor class
SharpDistSensor sensor(sensorPin, medianFilterWindowSize);




void setup() 
{
  
  Serial.begin(115200);


Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
}


void loop()
{



  unsigned int distance = sensor.getDist();
  Serial.println(distance);

  OSCMessage msg("/InfraRedSensor/1");
    msg.add(distance);
    Udp.beginPacket(DestIp, DestPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(10);
  
}
