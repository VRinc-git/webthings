#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>



#define BUILT_IN_LED   2              //Wifi indication LED

#define PIN1           39             //Switch 1
#define PIN2           34             //Switch 2
#define PIN3           35             //Switch 3
#define PIN4           32             //Switch 4

#define LED_PIN_1      33             //Relay 1
#define LED_PIN_2      25             //Relay 2
#define LED_PIN_3      26             //Relay 3
#define LED_PIN_4      27             //Relay 4

#define BUTTON         19             //Wifi reconnect

#define RGB_B          18             //Blue LED
#define RGB_G          5              //Green LED
#define RGB_R          17             //Red LED

#define BUZZ           16             //Buzzer

bool flag1_previous_I = 0;
bool flag1_previous_II = 1;
bool flag2_previous_I = 0;
bool flag2_previous_II = 1;
bool flag3_previous_I = 0;
bool flag3_previous_II = 1;
bool flag4_previous_I = 0;
bool flag4_previous_II = 1;

bool man_flag1_previous_I = 0;
bool man_flag1_previous_II = 1;
bool man_flag2_previous_I = 0;
bool man_flag2_previous_II = 1;
bool man_flag3_previous_I = 0;
bool man_flag3_previous_II = 1;
bool man_flag4_previous_I = 0;
bool man_flag4_previous_II = 1;

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
const char *ledTypes_1[] = {"onoffswitch",
                            "Light",
                            nullptr
                           };

const char *ledTypes_2[] = {"onoffswitch",
                            "Light",
                            nullptr
                           };

const char *ledTypes_3[] = {"onoffswitch",
                            "Light",
                            nullptr
                           };

const char *ledTypes_4[] = {"onoffswitch",
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
ThingDevice led_1("led 1", "Lamp", ledTypes_1);
ThingDevice led_2("led 2", "Lamp", ledTypes_2);
ThingDevice led_3("led 3", "Lamp", ledTypes_3);
ThingDevice led_4("led 4", "Lamp", ledTypes_4);


/*
   Define one or more properties supported by your Thing
   ThingProperty property(id, description, type, atType)
      @id: unique identifier for property
      @description: user-readable description of property
      @type: NO_STATE, BOOLEAN, NUMBER, or STRING
      @atType: property @type (https://iot.mozilla.org/schemas#properties)
*/

ThingProperty ledOn_1("on", "", BOOLEAN, "OnOffProperty");
ThingProperty ledOn_2("on", "", BOOLEAN, "OnOffProperty");
ThingProperty ledOn_3("on", "", BOOLEAN, "OnOffProperty");
ThingProperty ledOn_4("on", "", BOOLEAN, "OnOffProperty");


bool lastOn_1 = false;       // setting the value as 0
bool lastOn_2 = false;
bool lastOn_3 = false;
bool lastOn_4 = false;



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
  led_1.addProperty(&ledOn_1);               //@params : passing address of the object
  led_2.addProperty(&ledOn_2);
  led_3.addProperty(&ledOn_3);
  led_4.addProperty(&ledOn_4);



  /*
     Associate device with connection
  */
  adapter->addDevice(&led_1);
  adapter->addDevice(&led_2);
  adapter->addDevice(&led_3);
  adapter->addDevice(&led_4);




  /*
     start mDNS and HTTP server
  */

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led_1.id);

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led_2.id);

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led_3.id);

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led_4.id);

}


void setup()
{
  pinMode(BUILT_IN_LED, OUTPUT);
  digitalWrite(BUILT_IN_LED, HIGH);

  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);



  //user defined pins
  pinMode(PIN1, INPUT);
  pinMode(PIN2, INPUT);
  pinMode(PIN3, INPUT);
  pinMode(PIN4, INPUT);

  pinMode(LED_PIN_1, OUTPUT);
  digitalWrite(LED_PIN_1, HIGH);
  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_2, HIGH);
  pinMode(LED_PIN_3, OUTPUT);
  digitalWrite(LED_PIN_3, HIGH);
  pinMode(LED_PIN_4, OUTPUT);
  digitalWrite(LED_PIN_4, HIGH);

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


/*
   Adapter function
*/
void Adapter()
{
  ThingPropertyValue tpVal1, tpVal2, tpVal3, tpVal4;
  bool pin1, pin2, pin3, pin4;

  // Reading the values
  pin1 = digitalRead(PIN1);
  pin2 = digitalRead(PIN2);
  pin3 = digitalRead(PIN3);
  pin4 = digitalRead(PIN4);


  //Serial.println(pin);
  /*
     Relay 1
  */
  if ( pin1 == LOW)
  {
    tpVal1.boolean = pin1;
    if ( flag1_previous_I == 0)
    {

      ledOn_1.setValue(tpVal1);

      bool On1_1 = ledOn_1.getValue().boolean;
      digitalWrite(LED_PIN_1, On1_1 ? HIGH : LOW);

      if (On1_1 != lastOn_1)
      {
        Serial.print(led_1.id);
        Serial.print(": ");
        Serial.println(On1_1);
        Serial.println("condition 1");
      }
      lastOn_1 = On1_1;
      flag1_previous_I = 1;
      flag1_previous_II = 1;
    }

  }

  if ( pin1 == HIGH)
  {
    tpVal1.boolean = pin1;
    if ( flag1_previous_II == 1)
    {

      ledOn_1.setValue(tpVal1);

      bool On1_1 = ledOn_1.getValue().boolean;
      digitalWrite(LED_PIN_1, On1_1 ? HIGH : LOW);

      if (On1_1 != lastOn_1)
      {
        Serial.print(led_1.id);
        Serial.print(": ");
        Serial.println(On1_1);
        Serial.println("condition 2");
      }
      lastOn_1 = On1_1;
      flag1_previous_I = 0;
      flag1_previous_II = 0;
    }

  }
  bool On2_1 = ledOn_1.getValue().boolean;
  digitalWrite(LED_PIN_1, On2_1 ? LOW : HIGH);

  if (On2_1 != lastOn_1)
  {
    Serial.print(led_1.id);
    Serial.print(": ");
    Serial.println(On2_1);
    Serial.println("condition 3");
  }
  lastOn_1 = On2_1;
  //Serial.println(lastOn_1);

  /*
       Relay 2
  */
  if ( pin2 == LOW)
  {
    tpVal2.boolean = pin2;
    if ( flag2_previous_I == 0)
    {

      ledOn_2.setValue(tpVal2);

      bool On1_2 = ledOn_2.getValue().boolean;
      digitalWrite(LED_PIN_2, On1_2 ? LOW : HIGH);

      if (On1_2 != lastOn_2)
      {
        Serial.print(led_2.id);
        Serial.print(": ");
        Serial.println(On1_2);
        Serial.println("condition 1");
      }
      lastOn_2 = On1_2;
      flag2_previous_I = 1;
      flag2_previous_II = 1;
    }

  }

  if ( pin2 == HIGH)
  {
    tpVal2.boolean = pin2;
    if ( flag2_previous_II == 1)
    {

      ledOn_2.setValue(tpVal2);

      bool On1_2 = ledOn_2.getValue().boolean;
      digitalWrite(LED_PIN_2, On1_2 ? LOW : HIGH);

      if (On1_2 != lastOn_2)
      {
        Serial.print(led_2.id);
        Serial.print(": ");
        Serial.println(On1_2);
        Serial.println("condition 2");
      }
      lastOn_2 = On1_2;
      flag2_previous_I = 0;
      flag2_previous_II = 0;
    }

  }
  bool On2_2 = ledOn_2.getValue().boolean;
  digitalWrite(LED_PIN_2, On2_2 ? LOW : HIGH);

  if (On2_2 != lastOn_2)
  {
    Serial.print(led_2.id);
    Serial.print(": ");
    Serial.println(On2_2);
    Serial.println("condition 3");
  }
  lastOn_2 = On2_2;
  //Serial.println(lastOn_2);
  /*
       Relay 3
  */
  if ( pin3 == LOW)
  {
    tpVal3.boolean = pin3;
    if ( flag3_previous_I == 0)
    {

      ledOn_3.setValue(tpVal3);

      bool On1_3 = ledOn_3.getValue().boolean;
      digitalWrite(LED_PIN_3, On1_3 ? LOW : HIGH);

      if (On1_3 != lastOn_3)
      {
        Serial.print(led_3.id);
        Serial.print(": ");
        Serial.println(On1_3);
        Serial.println("condition 1");
      }
      lastOn_3 = On1_3;
      flag3_previous_I = 1;
      flag3_previous_II = 1;
    }

  }

  if ( pin3 == HIGH)
  {
    tpVal3.boolean = pin3;
    if ( flag3_previous_II == 1)
    {

      ledOn_3.setValue(tpVal3);

      bool On1_3 = ledOn_3.getValue().boolean;
      digitalWrite(LED_PIN_3, On1_3 ? LOW : HIGH);

      if (On1_3 != lastOn_3)
      {
        Serial.print(led_3.id);
        Serial.print(": ");
        Serial.println(On1_3);
        Serial.println("condition 2");
      }
      lastOn_3 = On1_3;
      flag3_previous_I = 0;
      flag3_previous_II = 0;
    }

  }
  bool On2_3 = ledOn_3.getValue().boolean;
  digitalWrite(LED_PIN_3, On2_3 ? LOW : HIGH);

  if (On2_3 != lastOn_3)
  {
    Serial.print(led_3.id);
    Serial.print(": ");
    Serial.println(On2_3);
    Serial.println("condition 3");
  }
  lastOn_3 = On2_3;
  // Serial.println(lastOn_3);
  /*
       Relay 4
  */
  if ( pin4 == LOW)
  {
    tpVal4.boolean = pin4;
    if ( flag4_previous_I == 0)
    {

      ledOn_4.setValue(tpVal4);

      bool On1_4 = ledOn_4.getValue().boolean;
      digitalWrite(LED_PIN_4, On1_4 ? LOW : HIGH);

      if (On1_4 != lastOn_4)
      {
        Serial.print(led_4.id);
        Serial.print(": ");
        Serial.println(On1_4);
        Serial.println("condition 1");
      }
      lastOn_4 = On1_4;
      flag4_previous_I = 1;
      flag4_previous_II = 1;
    }

  }

  if ( pin4 == HIGH)
  {
    tpVal4.boolean = pin4;
    if ( flag4_previous_II == 1)
    {

      ledOn_4.setValue(tpVal4);

      bool On1_4 = ledOn_4.getValue().boolean;
      digitalWrite(LED_PIN_4, On1_4 ? LOW : HIGH);

      if (On1_4 != lastOn_4)
      {
        Serial.print(led_4.id);
        Serial.print(": ");
        Serial.println(On1_4);
        Serial.println("condition 2");
      }
      lastOn_4 = On1_4;
      flag4_previous_I = 0;
      flag4_previous_II = 0;
    }

  }
  bool On2_4 = ledOn_4.getValue().boolean;
  digitalWrite(LED_PIN_4, On2_4 ? LOW : HIGH);

  if (On2_4 != lastOn_4)
  {
    Serial.print(led_4.id);
    Serial.print(": ");
    Serial.println(On2_4);
    Serial.println("condition 3");
  }
  lastOn_4 = On2_4;
  //  Serial.println(lastOn_4);
}




/*
    Function for manual control
    Incase of WiFi network got disconnected(not present), this can be used to control manually
*/
void Call_ManualControl()
{
  Serial.println("Manual Control");

  bool pin1 = digitalRead(PIN1);
  bool pin2 = digitalRead(PIN2);
  bool pin3 = digitalRead(PIN3);
  bool pin4 = digitalRead(PIN4);
  /*
     Relay 1
  */
  if ( pin1 == LOW)
  {
    if ( man_flag1_previous_I == 0)
    {
      digitalWrite(LED_PIN_1, pin1 ? LOW : HIGH);
      Serial.println("LED1");
      Serial.println(digitalRead(PIN1));
      man_flag1_previous_I = 1;
      man_flag1_previous_II = 1;
    }
  }
  if ( pin1 == HIGH)
  {
    if ( man_flag1_previous_II == 1)
    {
      digitalWrite(LED_PIN_1, pin1 ? LOW : HIGH);
      Serial.println("LED1");
      Serial.println(digitalRead(PIN1));
      man_flag1_previous_I = 0;
      man_flag1_previous_II = 0;
    }

  }
  /*
    Relay 2
  */
  if ( pin2 == LOW)
  {
    if ( man_flag2_previous_I == 0)
    {
      digitalWrite(LED_PIN_2, pin2 ? LOW : HIGH);
      Serial.println("LED2");
      Serial.println(digitalRead(PIN2));
      man_flag2_previous_I = 1;
      man_flag2_previous_II = 1;
    }
  }
  if ( pin2 == HIGH)
  {
    if ( man_flag2_previous_II == 1)
    {
      digitalWrite(LED_PIN_2, pin2 ? LOW : HIGH);
      Serial.println("LED2");
      Serial.println(digitalRead(PIN2));
      man_flag2_previous_I = 0;
      man_flag2_previous_II = 0;
    }

  }
  /*
    Relay 3
  */
  if ( pin3 == LOW)
  {
    if ( man_flag3_previous_I == 0)
    {
      digitalWrite(LED_PIN_3, pin3 ? LOW : HIGH);
      Serial.println("LED3");
      Serial.println(digitalRead(PIN3));
      man_flag3_previous_I = 1;
      man_flag3_previous_II = 1;
    }
  }
  if ( pin3 == HIGH)
  {
    if ( man_flag3_previous_II == 1)
    {
      digitalWrite(LED_PIN_3, pin3 ? LOW : HIGH);
      Serial.println("LED3");
      Serial.println(digitalRead(PIN3));
      man_flag3_previous_I = 0;
      man_flag3_previous_II = 0;
    }

  }
  /*
    Relay 4
  */
  if ( pin4 == LOW)
  {
    if ( man_flag4_previous_I == 0)
    {
      digitalWrite(LED_PIN_4, pin4 ? LOW : HIGH);
      Serial.println("LED4");
      Serial.println(digitalRead(PIN4));
      man_flag4_previous_I = 1;
      man_flag4_previous_II = 1;
    }
  }
  if ( pin4 == HIGH)
  {
    if ( man_flag4_previous_II == 1)
    {
      digitalWrite(LED_PIN_4, pin4 ? LOW : HIGH);
      Serial.println("LED4");
      Serial.println(digitalRead(PIN4));
      man_flag4_previous_I = 0;
      man_flag4_previous_II = 0;
    }

  }
  delay(200);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
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
