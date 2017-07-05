# Project-
Arduino 
#include <WiFi101.h>
#include <DHT.h>
#include <ArduinoJson.h>
// Web REST API params
/
char server[] = "api.artik.cloud";  //server address
int port = 443; //(port 443 is default for HTTPS) 
String AuthorizationData = "Authorization: Bearer 23d3833b58cf4901bbd4a78fda6ab073"; //token
/
int server = (192,168,254,110);
int port = 4567;
String AuthorizationData = "Authorization: Bearer 23d3833b58cf4901bbd4a78fda6ab073";
float insTemp;
char buf[200];
int ledState = 0;
int LED = 6;
int vibr_Pin = A0;
WiFiSSLClient client;
char ssid[] = "Athena";
char pass[] = "2SmallCarp";
int keyIndex = 0;
int status = WL_IDLE_STATUS;
#define DHTPIN 4    
#define DHTTYPE DHT22
DHT dht(DHTPIN,DHTTYPE);
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(vibr_Pin, INPUT); //set vibr_Pin input for measurment
  Serial.begin(9600);
  while (status != WL_CONNECTED){
    Serial.println("Attempting to connect to WiFi");
    status = WiFi.begin(ssid,pass);
  }
  Serial.println("Connected to WiFi ");
  printWifiStatus();
}
void loop() {
  // ping calls
  Serial.print("PING:");
  Serial.println(WiFi.ping(server));
  // assigned local ip
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address : ");
  Serial.println(ip);
  Serial.println(" ");
  Serial.println("Data");
  Serial.println("------------------------------------------------------------------------------------------------------------------");
  ledToggle();
  client.connect(server, port);
  //delay(1000);
  if (!client.connected()) { 
    Serial.println(" Error: Retrying Getting Data ");
  } else {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    long measurement = TP_init();
    Serial.println("Sending data Temperature(°C)" +String(t));
    Serial.println("Sending data Humidity(%) "+String(h));
    Serial.println("Sending data Vibration(Hz) "+String(measurement));
    client.println("POST /v1.1/messages HTTP/1.1");
    client.println("Host: api.artik.cloud");
    client.println("Accept: /");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println(AuthorizationData);
client.print("Content-Length: ");
client.println(loadBuffer(t,h,measurement)); 
client.println();
client.println(buf);
Serial.println(buf);
Serial.println("Data posted");
client.stop();       
  }
 //delay(1000); 
}
int loadBuffer(float insTemp, float insHumidity, float measurement) {
  StaticJsonBuffer<200> jsonBuffer; 
  JsonObject& root = jsonBuffer.createObject(); 
  root["sdid"] = "79a1f1cdbbbd4d8ca6e7e121e7b7c46d";  //device id
  root["type"] = "message";
  JsonObject& dataPair = root.createNestedObject("data");
  dataPair["temp"] = insTemp;
  dataPair["humidity"] = insHumidity;
  dataPair["vibration"] = measurement;
  root.printTo(buf, sizeof(buf));  //JSON print
  return (root.measureLength()); 
}
void ledToggle(){
  if (ledState == 0){
    digitalWrite(LED,LOW);
    ledState = 1;
  } else {
    digitalWrite(LED,HIGH); 
    ledState = 0;
  }
}
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
long TP_init(){
  long measurement=pulseIn (vibr_Pin, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}

//These are just defualt codes
