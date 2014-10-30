#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

//int RELAY_PIN = 3;
const int RELAY_COUNT = 2;
int RELAY_PIN[] = {3,4}; // your LED PIN
int LED_PIN = 13; // your LED PIN

boolean RELAY_ON = false;

YunServer server;

void setup() {
  // Start our connection
  delay(2000);
  Serial.begin(9600);
  
  for (int i =0; i<RELAY_COUNT; i++) {
    pinMode(RELAY_PIN[i],OUTPUT);
  //TURN RELAY OFF
    digitalWrite(RELAY_PIN[i],HIGH); // turn on Led while connecting
  }
  Bridge.begin();  

  // Show a fancy flash pattern once connected
  digitalWrite(LED_PIN,LOW); 
  delay(150);
  digitalWrite(LED_PIN,HIGH); 
  delay(150);
  digitalWrite(LED_PIN,LOW); 
  delay(150);
  digitalWrite(LED_PIN,HIGH); 
  delay(150);
  digitalWrite(LED_PIN,LOW); 
  delay(150);
  
  //digitalWrite(RELAY_PIN,HIGH);
  
  // Disable for some connections:
  // Start listening for connections  
  
  // server.listenOnLocalhost();
  server.begin();
 
}

void loop() {
  // Listen for clients
  YunClient client = server.accept();
  // Client exists?
  if (client) {
    // Lets process the request!
    process(client);
    client.stop();
  }
  delay(50);
  serialEvent();
}

void process(YunClient client) {
  // Collect user commands
  String command = client.readStringUntil('\\'); // load whole string
  command.trim();
  // Enable HTML
  
  
    // Turn on
  if (command == "on") {
    //digitalWrite(3,HIGH);   
    setRelayPins(LOW);
    RELAY_ON = true;
  }
  
  // Turn off
  if (command == "off") {
    //digitalWrite(3,LOW);    
    setRelayPins(HIGH);
    RELAY_ON = false;
  }

  client.println("Status: 200");
  client.println("Content-type: text/html");
  client.println();
  client.println("<style type='text/css'>");
  client.println("center { font-size:48px; }");
  client.println("</style>");
  
  // Show UI
  client.println("<B><Center>");
  if (RELAY_ON) {
     client.println("LIGHTSWARM is ON<br><br>");
  } else {
     client.println("LIGHTSWARM is OFF<br><br>");
  }
  client.println("<a href='/arduino/on\\'>Turn ON LED</a><br>");
  client.println("<a href='/arduino/off\\'>Turn OFF LED</a><br>");
  //client.println("<a href='http://lightswarm.local/arduino/off\\'>Turn OFF LED</a><br>");
  client.print("Command: ");
  client.println(command);
  client.println("</B></Center>");
  
  // Check what the user entered ...
  Serial.print("COMMAND -->");
  Serial.print(command);
  Serial.println("END");

}

void setRelayPins(int value) {
   for (int i =0; i<RELAY_COUNT; i++) {
      digitalWrite(RELAY_PIN[i],value); // turn on Led while connecting
      digitalWrite(LED_PIN, !value);
  }
}

void serialEvent() {
  if (Serial.available() > 0) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    
    if (inChar == 'Q') {    
      setRelayPins(HIGH);
      RELAY_ON = false;
    }
    
    if (inChar == 'B') { 
      setRelayPins(LOW);
      RELAY_ON = true;
    }
    
    
  }
}



