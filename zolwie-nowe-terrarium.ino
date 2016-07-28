#include <Thread.h>
#include <ThreadController.h>

#include "etherShield.h"
#include "ETHER_28J60.h"
#include <idDHT11.h>

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};
static uint8_t ip[4] = {192, 168, 1, 100};
static uint16_t port = 80;

ETHER_28J60 e;

int idDHT11pin = 2; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)

//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(idDHT11pin, idDHT11intNumber, dht11_wrapper);

byte current_temp, current_humidity;

Thread api_thread, temp_thread = Thread();

char *params;

void setup(){ 
  delay(1000);
  e.setup(mac, ip, port);
  Serial.begin(38400);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  //api_thread
  api_thread.setInterval(500); // Setts the wanted interval to be 10ms
  // This will set the callback of the Thread: "What should I run"?
  api_thread.onRun(api); // callback_function is the name of the function
  
  //temp_thread
  temp_thread.setInterval(10000); // Setts the wanted interval to be 10ms
  // This will set the callback of the Thread: "What should I run"?
  temp_thread.onRun(temperature_and_humidity); // callback_function is the name of the function
  
  current_temp = 0;
  current_humidity = 0;
}

// This wrapper is in charge of calling 
// mus be defined like this for the lib work
void dht11_wrapper() {
  DHT11.isrCallback();
}

void loop(){
  if(api_thread.shouldRun()){
    api_thread.run();
  }
  if(temp_thread.shouldRun()){
    temp_thread.run();
  }
}

void api(){
  if(params = e.serviceRequest()) {
    if (strcmp(params, "heat_lamp/1") == 0) {
      digitalWrite(3, HIGH);
      e.print("Lampka grzewcza wlaczona");
    } else if (strcmp(params, "heat_lamp/0") == 0) {
      digitalWrite(3, LOW);
      e.print("Lampka grzewcza wlaczona");
    } else if (strcmp(params, "light_for_plants/1") == 0) {
      digitalWrite(4, HIGH);
      e.print("Lampka do roslin wlaczona");
    } else if (strcmp(params, "light_for_plants/0") == 0) {
      digitalWrite(4, LOW);
      e.print("Lampka do roslin wlaczona");
    } else if (strcmp(params, "fogger/1") == 0) {
      digitalWrite(5, HIGH);
      e.print("Zmglawiacz wlaczony");
    } else if (strcmp(params, "fogger/0") == 0) {
      digitalWrite(5, LOW);
      e.print("Zmglawiacz wylaczony");
    } else {
      e.print("Wilgotnosc: "); 
      e.print(current_humidity);
      e.print("Temperatura: "); 
      e.print(current_temp);
    }
    e.respond();
  }
}

void temperature_and_humidity(){
  DHT11.acquire();
  while (DHT11.acquiring());
  int result = DHT11.getStatus();
  current_humidity = DHT11.getHumidity();
  current_temp = DHT11.getCelsius();
  delay(1);
}



