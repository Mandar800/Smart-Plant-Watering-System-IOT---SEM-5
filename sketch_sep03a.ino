#include <SoftwareSerial.h>
#define RX 10
#define TX 11
String AP = "redmi";       // CHANGE ME
String PASS = "123456789"; // CHANGE ME
String API = "BQHV4Z9QB659WL61";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field2 = "field2";
String field1 = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor1 = 1;
int valSensor2 = 1;
SoftwareSerial esp8266(RX,TX); 
const int lm35_pin = A1;
int temp_adc_val;
float temp_val;
const int sensor_pin = A0;
float moisture_percentage;
int sensor_analog; 
float vref = 3.3;
float resolution = vref/1023; 
int motorPin = A2;
 
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  pinMode(motorPin, OUTPUT);
}
void loop() {
  //temp value
 valSensor1 = getSensorData1();
 String getData = "GET /update?api_key="+ API +"&"+ field2 +"="+String(valSensor1);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
//moisture value
  valSensor2 = getSensorData2();
 String getData2 = "GET /update?api_key="+ API +"&"+ field1 +"="+String(valSensor2);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData2.length()+4),4,">");
 esp8266.println(getData2);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 if(valSensor2<60){
    digitalWrite(A2, HIGH);
    while(valSensor2<60){valSensor2 = getSensorData2();}
    digitalWrite(A2,LOW);
   
    }
    
  
}
int getSensorData1(){
  
float temperature = analogRead(A1);
 temperature = (temperature*500)/1023;
 //temperature = temperature*100;
 Serial.print("Temperature is : ");
 Serial.println(temperature);
  
  return temperature; 
}
int getSensorData2(){
  
 sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");
 
  return moisture_percentage; 
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
