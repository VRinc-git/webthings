#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>


#define BUILT_IN_LED   2
#define PIN            34
#define LED_PIN        33
#define BUTTON         25

int flag_previous_I = 0;
int flag_previous_II = 1;



/*
   SSID and Password
*/
const char *ssid = "VR";
const char *password = "wpa2 psk";


/*
   Host name used by mDNS
*/
const String mDNSHostname = "RealTimeFeedback";



/*
   Handle connection between things and gateway
*/

WebThingAdapter *adapter;



/*
   @type members: Capabilities supported by your Thing
   See schemas: https://iot.mozilla.org/schemas#capabilities
*/
const char *ledTypes[] = {"onoffswitch",
                          "Light",
                          nullptr
                         };


/*
   Description of your Thing

   ThingDevice device(id, title, types)

      @id: unique identifier for Thing (part of URL: http://<IP>/things/<id>)
      @description: string that shows up in Gateway for your Thing
      @types: array of @types
*/
ThingDevice led("led", "Lamp", ledTypes);



/*
   Define one or more properties supported by your Thing

   ThingProperty property(id, description, type, atType)

      @id: unique identifier for property
      @description: user-readable description of property
      @type: NO_STATE, BOOLEAN, NUMBER, or STRING
      @atType: property @type (https://iot.mozilla.org/schemas#properties)
*/

ThingProperty ledOn("on", "", BOOLEAN, "OnOffProperty");


bool lastOn = false;       // setting the value as 0




/*
   WiFi setup
*/
void WiFiSetup()
{
  int interval = 1000;
  int previousMillis = 0;
  bool blink = true;
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(BUILT_IN_LED, blink ? LOW : HIGH);     //Blink until the WiFi get connected
    blink = !blink;
    if (WiFi.status() == WL_CONNECTED)
    {
      break;                                            // if WiFi got connected then Break
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      Call_ManualControl();                             // if not connected then call Manual Control
    }

  }
  digitalWrite(BUILT_IN_LED, HIGH);
  AdapterSetup();
}


/*
   Adapter Setup
*/
void AdapterSetup()
{
  Serial.println("");
  Serial.print("Connected!");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  /*
    Create new WebThing connection handle (default port: 80)
  */
  adapter = new WebThingAdapter(mDNSHostname, WiFi.localIP());



  /*
     Associate properties with devices
  */
  led.addProperty(&ledOn);               //@params : passing address of the object




  /*
     Associate device with connection
  */
  adapter->addDevice(&led);




  /*
     start mDNS and HTTP server
  */

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led.id);


}


void setup()
{
  pinMode(BUILT_IN_LED, OUTPUT);
  digitalWrite(BUILT_IN_LED, HIGH);

  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);



  //user defined pins
  pinMode(PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  //Setting up serial
  Serial.begin(115200);

  Serial.print("Connecting to");
  Serial.print(ssid);

#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");


  //Setting up WiFi
  WiFiSetup();


}


void Adapter()
{
  ThingPropertyValue tpVal;
  bool pin;

  // Reading the values
  pin = digitalRead(PIN);
  //Serial.println(pin);

  if ( pin == LOW)
  {
    tpVal.boolean = pin;
    if ( flag_previous_I == 0)
    {

      ledOn.setValue(tpVal);

      bool On1 = ledOn.getValue().boolean;
      digitalWrite(LED_PIN, On1 ? LOW : HIGH);

      if (On1 != lastOn)
      {
        Serial.print(led.id);
        Serial.print(": ");
        Serial.println(On1);
        Serial.println("condition 1");
      }
      lastOn = On1;
      flag_previous_I = 1;
      flag_previous_II = 1;
    }

  }

  if ( pin == HIGH)
  {
    tpVal.boolean = pin;
    if ( flag_previous_II == 1)
    {

      ledOn.setValue(tpVal);

      bool On1 = ledOn.getValue().boolean;
      digitalWrite(LED_PIN, On1 ? LOW : HIGH);

      if (On1 != lastOn)
      {
        Serial.print(led.id);
        Serial.print(": ");
        Serial.println(On1);
        Serial.println("condition 2");
      }
      lastOn = On1;
      flag_previous_I = 0;
      flag_previous_II = 0;
    }

  }
  bool On2 = ledOn.getValue().boolean;
  digitalWrite(LED_PIN, On2 ? LOW : HIGH);

  if (On2 != lastOn)
  {
    Serial.print(led.id);
    Serial.print(": ");
    Serial.println(On2);
    Serial.println("condition 3");
  }
  lastOn = On2;
  Serial.println(lastOn);
}


void loop()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    Call_ManualControl();
    if (digitalRead(BUTTON) == LOW)
    {
      Serial.println("restarting ESP");
      ESP.restart();
    }
  }

  Adapter();
  





  // Update all the properties and events
  adapter->update();

  // Wait for some time
  delay(2000);

}



/*
    Function for manual control
    Incase of WiFi network got disconnected(not present), this can be used to control manually
*/
void Call_ManualControl()
{
  Serial.println("Manual Control");
  digitalWrite(LED_PIN, digitalRead(PIN));
  Serial.println("LED");
  Serial.println(digitalRead(PIN));
  delay(1);
}
