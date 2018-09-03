#include <nRF905.h>
#define RXADDR 0xFE4CA6E5 // Address of this device
#define TXADDR 0x586F2E10 // Address of device to send to

#define TIMEOUT 1000

#define PACKET_NONE 0
#define PACKET_OK 1
#define PACKET_INVALID 2

#define NRF905_MAX_PAYLOAD 32

static volatile uint8_t packetStatus;


void NRF905_CB_RXCOMPLETE(void)
{
  packetStatus = PACKET_OK;
}

void NRF905_CB_RXINVALID(void)
{
  packetStatus = PACKET_INVALID;
}

void setup() {
  //Set up the serial port
  Serial.begin(115200);
  Serial.println(F("Transmiter is up"));

  //Initiate communication
  nRF905_init();

  //Set receaver address
  // Set address of this device
  nRF905_setListenAddress(RXADDR);

}

void loop() {
  // Make data
  char data[NRF905_MAX_PAYLOAD] = {0};
  sprintf(data, "test");

  //Reset packet status
  packetStatus = PACKET_NONE;

  //Send the data (send fails if other transmissions are going on, keep trying until success) and enter RX mode on completion
  while(!nRF905_TX(TXADDR, data, sizeof(data), NRF905_NEXTMODE_RX));

  uint8_t success;
  // Wait for reply
  uint32_t sendStartTime = millis();
  while(1)
  {
    success = packetStatus;
    if(success != PACKET_NONE)
      break;
    else if(millis() - sendStartTime > TIMEOUT)
      break;
  }

  if(success == PACKET_OK)
  {
    
  }

}

