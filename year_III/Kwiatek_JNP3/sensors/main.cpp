#include <string>
#include "mbed.h"
#include "nRF24L01P.h"
#include "cantcoap.h"

//***************************************************************************//
// !!! SETTINGS !!!

#define DATA_RATE       NRF24L01P_DATARATE_250_KBPS
#define POWER           NRF24L01P_TX_PWR_ZERO_DB
#define CHANNEL         10

#define RX_ADDRESS      ((unsigned long long) 0xABCDEF01)
#define TX_ADDRESS      ((unsigned long long) 0xABCDEF00)

#define TRANSFER_SIZE   32


//#define SOIL_MOISTURE
//#define LIGHT_SENSOR
//#define TEMPERATURE_SENSOR
//#define BUTTON // If I have button i have led also
//***************************************************************************//
// Radio variables
nRF24L01P radio(PB_15, PB_14, PB_13, PB_12, PB_1, PB_2); 

// CoAP variables
const char* soil_s = "ms/";
const char* light_s = "ls/";
const char* temp_s = "ts/";

// Sensor and general variables
#ifdef SOIL_MOISTURE
    AnalogIn soil_moisture(PA_0);
    double soil_read, soil;
#endif
#ifdef LIGHT_SENSOR
    AnalogIn light_sensor(PA_1);
    double light_read, light;
#endif
#ifdef TEMPERATURE_SENSOR
    AnalogIn temperature_sensor(PA_4);
    double temp_read, temp_c;
#endif

Ticker waker;
Serial pc(USBTX, USBRX);
bool expired = false;

#define TEMP_RATIO_C 3300/10.24

double wait_time = 2.0;

#ifdef BUTTON
InterruptIn button_int(D3);
DigitalOut led_light(D2);

bool led_is_on = false;

void button_pressed()
{
    led_is_on = !led_is_on;
    led_light.write(led_is_on);
}

void button_released() {}
#endif

void wake() {
    expired = true;
}

void send(uint8_t* msg_u, int msg_size){
    char* msg = (char*) msg_u;
    pc.printf("msg size: %d \r\n", msg_size);
    int tx_bytes = radio.write(NRF24L01P_PIPE_P0, msg, msg_size); // TODO Check if change to TRANSFER_SIZE helps
    pc.printf("tx_bytes: %d \r\n", tx_bytes);
    if(tx_bytes < 0)
        pc.printf("TX ERROR\r\n");
}

void compose_message(char* data, float fl, const char* c) {
    unsigned long long rx = RX_ADDRESS;
    memset(data, 0, TRANSFER_SIZE);
    memcpy(data, &fl, 4);
    memcpy(data+4, c, 1);
    memcpy(data+5, &rx, 8);
}


int main()
{
    pc.baud(115200);
    waker.attach(&wake, wait_time);
    
    //Radio
    char txData[TRANSFER_SIZE];
    radio.powerDown();
    radio.powerUp();
    
    radio.setAirDataRate(DATA_RATE);
    radio.setRfOutputPower(POWER);
    radio.setRfFrequency(NRF24L01P_MIN_RF_FREQUENCY + 4 * CHANNEL);
    
    radio.setCrcWidth(NRF24L01P_CRC_8_BIT);
    radio.enableAutoAcknowledge(NRF24L01P_PIPE_P0);
    radio.enableAutoAcknowledge(NRF24L01P_PIPE_P1);
    radio.enableDynamicPayload(NRF24L01P_PIPE_P0);
    radio.enableDynamicPayload(NRF24L01P_PIPE_P1);
    radio.enableAutoRetransmit(0x0F, 0x0F);
    
    radio.setTxAddress(TX_ADDRESS, 4);
    radio.setRxAddress(TX_ADDRESS, 4, NRF24L01P_PIPE_P0);
    
    radio.setRxAddress(RX_ADDRESS, 4, NRF24L01P_PIPE_P1);
    
    // Display the (default) setup of the nRF24L01+ chip
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  radio.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  radio.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", radio.getAirDataRate() );
    pc.printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", radio.getTxAddress() );
    pc.printf( "nRF24L01+ RX0 Address   : 0x%010llX\r\n", radio.getRxAddress(NRF24L01P_PIPE_P0) );
    pc.printf( "nRF24L01+ RX1 Address   : 0x%010llX\r\n", radio.getRxAddress(NRF24L01P_PIPE_P1) );

    radio.setTransferSize(TRANSFER_SIZE, NRF24L01P_PIPE_P0);
    radio.setTransferSize(TRANSFER_SIZE, NRF24L01P_PIPE_P1);

    radio.setReceiveMode();
    radio.enable();

#ifdef BUTTON
    button_int.fall(&button_pressed);
    button_int.rise(&button_released);
#endif
#ifdef SOIL_MOISTURE 
    int soil_message_id = 0;
    CoapPDU *soil_pdu = new CoapPDU();
    soil_pdu->setType(CoapPDU::COAP_NON_CONFIRMABLE);
    soil_pdu->setCode(CoapPDU::COAP_POST);
    soil_pdu->setToken((uint8_t*)"\3\2\1\0",4);
    soil_pdu->setURI((char*)soil_s, 3);
#endif
#ifdef LIGHT_SENSOR
    int light_message_id = 0;
    CoapPDU *light_pdu = new CoapPDU();
    light_pdu->setType(CoapPDU::COAP_NON_CONFIRMABLE);
    light_pdu->setCode(CoapPDU::COAP_POST);
    light_pdu->setToken((uint8_t*)"\3\2\1\0",4);
    light_pdu->setURI((char*)light_s, 3);
#endif
#ifdef TEMPERATURE_SENSOR
    int temp_message_id = 0;
    CoapPDU *temp_pdu = new CoapPDU();
    temp_pdu->setType(CoapPDU::COAP_NON_CONFIRMABLE);
    temp_pdu->setCode(CoapPDU::COAP_POST);
    temp_pdu->setToken((uint8_t*)"\3\2\1\0",4);
    temp_pdu->setURI((char*)temp_s, 3);
#endif

    while(1) {
#ifdef BUTTON
        pc.printf("BUTTON: %d \r\n", button_int.read());
#endif
#ifdef SOIL_MOISTURE
        soil_read = soil_moisture.read();
        pc.printf("SOIL_MOISTURE: %f \r\n", soil_read);
        
        soil_pdu->setMessageID(soil_message_id++);
        compose_message(txData, soil_read, "m");
        soil_pdu->setPayload((uint8_t*)txData, 13);
        send(soil_pdu->getPDUPointer(), soil_pdu->getPDULength());
#endif
#ifdef LIGHT_SENSOR
        light_read = light_sensor.read();
        pc.printf("LIGHT_SENSOR: %f \r\n", light_read);
        
        light_pdu->setMessageID(light_message_id++);
        compose_message(txData, light_read, "l");
        light_pdu->setPayload((uint8_t*)txData, 13);
        send(light_pdu->getPDUPointer(), light_pdu->getPDULength());
#endif
#ifdef TEMPERATURE_SENSOR
        temp_read = temperature_sensor.read();
        pc.printf("TEMPERATURE_SENSOR: %f \r\n", temp_read);
        temp_c = (double) temp_read * TEMP_RATIO_C; 
        pc.printf("TEMPERATURE_SENSOR: %f *C\r\n", temp_c);
        
        temp_pdu->setMessageID(temp_message_id++);
        compose_message(txData, temp_c, "t");
        temp_pdu->setPayload((uint8_t*)txData, 13);
        send(temp_pdu->getPDUPointer(), temp_pdu->getPDULength());
#endif
        
        pc.printf("Goint to sleep for: %f \r\n", wait_time);
        while(!expired) sleep();
        expired = false;
        
    }

}
