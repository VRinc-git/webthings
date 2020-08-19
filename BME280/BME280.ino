/*
   Webthing adapter - LM35 analog temperature sensor.
*/


#include <Arduino.h>
#include <Thing.h>
#include <WebThingAdapter.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


#define BUILT_IN_LED   2
/* SPI
  #define BME_SCK 18
  #define BME_MISO 19
  #define BME_MOSI 23
  #define BME_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)

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
const char *sensorTypes_1[] = {"TemperatureSensor", nullptr };
const char *sensorTypes_2[] = {"HumiditySensor", nullptr };
const char *sensorTypes_3[] = {"AltitudeSensor", nullptr };
const char *sensorTypes_4[] = {"PressureSensor", nullptr };

/*
   Description of your Thing
   ThingDevice device(id, title, types)
      @id: unique identifier for Thing (part of URL: http://<IP>/things/<id>)
      @description: string that shows up in Gateway for your Thing
      @types: array of @types
*/
ThingDevice sensor_1("BME280", "Temperature sensor", sensorTypes_1);
ThingDevice sensor_2("BME280", "Humidity sensor", sensorTypes_2);
ThingDevice sensor_3("BME280", "Altitude sensor", sensorTypes_3);
ThingDevice sensor_4("BME280", "Pressure sensor", sensorTypes_4);

/*
   Define one or more properties supported by your Thing
   ThingProperty property(id, description, type, atType)
      @id: unique identifier for property
      @description: user-readable description of property
      @type: NO_STATE, BOOLEAN, NUMBER, or STRING
      @atType: property @type (https://iot.mozilla.org/schemas#properties)
*/

ThingProperty sensorTemperature("Temperature", "", NUMBER, "TemperatureProperty");
ThingProperty sensorHumidity("Humidity", "", NUMBER, "TemperatureProperty");
ThingProperty sensorAltitude("Altitudei", "", NUMBER, "TemperatureProperty");
ThingProperty sensorPressure("Pressure", "", NUMBER, "TemperatureProperty");


/*
   Setup a BME280 instance to communicate with sensor
   Note: choose the correct protocol to communicate with the sensor
*/

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

void setup()
{
  pinMode(BUILT_IN_LED, OUTPUT);
  digitalWrite(BUILT_IN_LED, HIGH);

  //Setting up serial
  Serial.begin(115200);

  // start the sensor
  bool status;
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }


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
  sensorTemperature.unit = "C";
  sensorHumidity.unit = "%";
  sensorAltitude.unit = "m";
  sensorPressure.unit = "hPa";

  /*
     Associate properties with devices
  */
  sensor_1.addProperty(&sensorTemperature);               //@params : passing address of the object
  sensor_2.addProperty(&sensorHumidity);
  sensor_3.addProperty(&sensorAltitude);
  sensor_4.addProperty(&sensorPressure);

  /*
     Associate device with connection
  */
  adapter->addDevice(&sensor_1);
  adapter->addDevice(&sensor_2);
  adapter->addDevice(&sensor_3);
  adapter->addDevice(&sensor_4);

  /*
     start mDNS and HTTP server
  */

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(sensorTemperature.id);

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(sensorHumidity.id);

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(sensorAltitude.id);

  adapter->begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(sensorPressure.id);

}

void loop()
{
  adapter->update();

  ThingPropertyValue tpVal1, tpVal2, tpVal3, tpVal4;
  float sensor_value1, sensor_value2, sensor_value3, sensor_value4;


  // Reading the values
  sensor_value1 = bme.readTemperature();
  Serial.println(sensor_value1);
  // Update device values
  tpVal1.number = sensor_value1;
  sensorTemperature.setValue(tpVal1);

  sensor_value2 = bme.readPressure() / 100.0F;
  Serial.println(sensor_value2);
  // Update device values
  tpVal2.number = sensor_value2;
  sensorPressure.setValue(tpVal2);

  sensor_value3 = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.println(sensor_value3);
  // Update device values
  tpVal3.number = sensor_value3;
  sensorAltitude.setValue(tpVal3);

  sensor_value4 = bme.readHumidity();
  Serial.println(sensor_value4);
  // Update device values
  tpVal4.number = sensor_value4;
  sensorHumidity.setValue(tpVal4);


  // Update all the properties and events
  adapter->update();

  delay(2000);

}
