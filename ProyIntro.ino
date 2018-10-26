#include <Ethernet.h>
#include <SPI.h>
#include "DHT.h"

//Configuracion pin DHT
DHT dht(7, DHT11);

//Configuración pines Ultrasonido
const unsigned int TRIG_PIN=8;
const unsigned int ECHO_PIN=9;
const unsigned int BAUD_RATE=9600;

//Referencias de comparación
const unsigned int RefT=25;
const unsigned int RefNivel=5;
const unsigned int RefH=80;

//Configuración RED
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 }; // RESERVED MAC ADDRESS
EthernetClient client;

String data;

void setup() { 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(BAUD_RATE);

  // PREPARAR LA LIBRERIA PARA COMUNICARSE CON EL SENSOR
  dht.begin();

	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP"); 
	}
 
	data = "";
}

void loop(){
  delay(2000); // Esperar Entre Mediciones

  //Lectura de Datos
  
  // LEER LA HUMEDAD USANDO EL METRODO READHUMIDITY
  float h = dht.readHumidity();
  // LEER LA TEMPERATURA USANDO EL METRODO READTEMPERATURE
  float t = dht.readTemperature();
  // REVISAR QUE LOS RESULTADOS SEAN VALORES NUMERICOS VALIDOS, INDICANDO QUE LA COMUNICACION ES CORRECTA
  if (isnan(h) || isnan(t)) {
    Serial.println("Falla al leer el sensor DHT11!");
    return;
  }
  //Leer Sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
 
  const unsigned long duration= pulseIn(ECHO_PIN, HIGH);
  int distance= duration/29/2;
    

  
	data = "T=" + (String)t + "&H=" + (String)h + "&d=" + (String)distance;
  Serial.println(data); 

	if (client.connect("proyectoiieagricola.000webhostapp.com",80)) { // REPLACE WITH YOUR SERVER ADDRESS
		client.println("POST /Users.php HTTP/1.1"); 
		client.println("Host: proyectoiieagricola.000webhostapp.com"); // SERVER ADDRESS HERE TOO
		client.println("Content-Type: application/x-www-form-urlencoded"); 
		client.print("Content-Length: "); 
		client.println(data.length()); 
		client.println(); 
		client.print(data); 
    Serial.println(data); 
    Serial.println("Enviado"); 
	} 

	if (client.connected()) { 
		client.stop();	// DISCONNECT FROM THE SERVER
	}

	delay(3000); // WAIT FIVE MINUTES BEFORE SENDING AGAIN
}
