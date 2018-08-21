/*
 This is a test to check connection and functionalities to EmonCms.org
 using Industruino D21G with Ethernet card.
 At the moment, it fails to connect to emoncms.org. 
*/

#include <SPI.h>
#include <Ethernet2.h>  // this is the fork at https://github.com/Industruino/Ethernet2 
#define EMON_TOKEN "e5ffc481bc5724c68a96307ca47c6260"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:

//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)(just to test, it works)
//IPAddress server(217, 160, 223, 126); // this is the address of www.emoncms.org it connects but the server is not available on the other side
//IPAddress server(213, 138, 101, 177); // this is the address riported in industruino example. no reply
IPAddress server(80, 243, 190, 58); // this works with esp. it is the address of http://emoncms.org
//char server[] = "www.google.com";    // name address for Google (using DNS)- it works
//char server[] = "www.emoncms.org";  // it works, but no server on the other side
//char server[] = "emoncms.org";  // this does not connect

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient emon;

void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(9600);


  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    SerialUSB.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  SerialUSB.print("local IP: ");
  SerialUSB.println(Ethernet.localIP());
  // give the Ethernet shield a second to initialize:
  delay(1000);
  SerialUSB.print("connecting... ");
  SerialUSB.println(String(server));
  // if you get a connection, report back via serial:
  if (emon.connect(server, 80)) {  // here the procedure get stuck for several seconds and then returns "0" - i.e. connection failed
    SerialUSB.println("connected");


    // Make a HTTP request:
    String payload = "{\"deltapress1\":5}";
    SerialUSB.println(payload);
    emon.println(String("GET /input/post?json=") + payload + String("&apikey=") + String(EMON_TOKEN));
  }
  else {
    // if you didn't get a connection to the server:
    SerialUSB.println("connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (emon.available()) {
    char c = emon.read();
    SerialUSB.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!emon.connected()) {
    SerialUSB.println();
    SerialUSB.println("disconnecting.");
    emon.stop();

    // do nothing forevermore:
    while (true);
  }
}

