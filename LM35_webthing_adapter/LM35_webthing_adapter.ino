/*
   Webthing adapter - LM35 analog temperature sensor.
*/


#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>


#define BUILT_IN_LED   2
#define SENSOR_PIN     34

/*
   SSID and Password
*/
const char *ssid = "SSID";
const char *password = "PASSWORD";

/*
   Host name used by mDNS
*/
const String mDNSHostname = "Temperature sensor";

/*
   Handle connection between things and gateway
*/

WebThingAdapter *adapter;


/*
   @type members: Capabilities supported by your Thing
   See schemas: https://iot.mozilla.org/schemas#capabilities
*/
const char *sensorTypes[] = {"TemperatureSensor", nullptr };

/*
   Description of your Thing
   ThingDevice device(id, title, types)
      @id: unique identifier for Thing (part of URL: http://<IP>/things/<id>)
      @description: string that shows up in Gateway for your Thing
      @types: array of @types
*/
ThingDevice sensor("LM35", "Temperature sensor", sensorTypes);

/*
   Define one or more properties supported by your Thing
   ThingProperty property(id, description, type, atType)
      @id: unique identifier for property
      @description: user-readable description of property
      @type: NO_STATE, BOOLEAN, NUMBER, or STRING
      @atType: property @type (https://iot.mozilla.org/schemas#properties)
*/

ThingProperty sensorTemp("Temperature", "", NUMBER, "TemperatureProperty");


bool lastOn = false;       // setting the value as 0


void setup()
{
  pinMode(BUILT_IN_LED, OUTPUT);
  digitalWrite(BUILT_IN_LED, HIGH);



  //user defined pins
  pinMode(SENSOR_PIN, INPUT);

  //Setting up serial
  Serial.begin(115200);

  Serial.print("Connecting to");
  Serial.print(ssid);

#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");


  /*
     Waiting for connection
  */
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(BUILT_IN_LED, blink ? LOW : HIGH);
    blink = !blink;
  }
  digitalWrite(BUILT_IN_LED, HIGH);

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
     Set units for properties
  */
  sensorTemp.unit = "celsius";

  /*
     Associate properties with devices
  */
  sensor.addProperty(&sensorTemp);               //@params : passing address of the object


  /*
     Associate device with connection
  */
  adapter->addDevice(&sensor);

  /*
     start mDNS and HTTP server
  */

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(sensor.id);

}

void loop()
{
  adapter->update();

  ThingPropertyValue tpVal;
  float sensor_value;

  // Reading the values
  sensor_value = analogRead(SENSOR_PIN);
  Serial.println(sensor_value);

  // Updare device values
  tpVal.number = sensor_value;
  sensorTemp.setValue(tpVal);


  // Update all the properties and events
  adapter->update();

  delay(2000);

}
