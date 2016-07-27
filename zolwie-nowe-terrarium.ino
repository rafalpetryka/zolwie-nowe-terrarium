#include "etherShield.h"
#include "ETHER_28J60.h"

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};
static uint8_t ip[4] = {192, 168, 1, 100};
static uint16_t port = 80;

ETHER_28J60 e;

void setup()
{ 
  delay(1000);
  e.setup(mac, ip, port);
  Serial.begin(384000);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop()
{
  char* params;

  if(params = e.serviceRequest())
  {
    if (strcmp(params, "heat_lamp/1") == 0) {
      digitalWrite(2, HIGH);
      e.print("Lampka grzewcza włączona");
    } else if (strcmp(params, "heat_lamp/0") == 0) {
      digitalWrite(2, LOW);
      e.print("Lampka grzewcza wyłączona");
    } else if (strcmp(params, "light_for_plants/1") == 0) {
      digitalWrite(3, HIGH);
      e.print("Lampka do roślin włączona");
    } else if (strcmp(params, "light_for_plants/0") == 0) {
      digitalWrite(3, LOW);
      e.print("Lampka do roślin wyłączona");
    }
    else {
      e.print("Niepoprwany adres"); 
    }
    e.respond();
  }

}
