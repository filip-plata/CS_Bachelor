#include "mbed.h"
#include "nRF24L01P.h"
#include "cantcoap.h"

//***************************************************************************//
// !!! SETTINGS !!!

#define DATA_RATE       NRF24L01P_DATARATE_250_KBPS
#define POWER           NRF24L01P_TX_PWR_ZERO_DB
#define CHANNEL         10

#define RX_ADDRESS      ((unsigned long long) 0xABCDEF00)
#define TX_ADDRESS      ((unsigned long long) 0xABCDEF01)
#define ECHO_DELAY      0.5

#define TRANSFER_SIZE   32

//***************************************************************************//

Serial pc(USBTX, USBRX); // tx, rx
Serial wifi(PA_9, PA_10);
nRF24L01P radio(PB_15, PB_14, PB_13, PB_12, PB_1, PB_2);    // mosi, miso, sck, csn, ce, irq
DigitalOut packet_end(PC_3);

void wifi_write(char* rxData, int rxBytes) {
    wait(0.01);
    packet_end.write(0);
    wait(0.01);
    wifi.putc(0);
    for (int idx = 0; idx < rxBytes; idx++) {
        wifi.putc(rxData[idx]); 
    }
    wait(0.01);
    packet_end.write(1);
    wait(0.01);
}

int main() 
{
    char txData[TRANSFER_SIZE], rxData[TRANSFER_SIZE];

    pc.baud(115200);
    wifi.baud(115200);
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

    while (1) {
        int rx_bytes=0;

        if(radio.readable(NRF24L01P_PIPE_P1)){
            rx_bytes = radio.read(NRF24L01P_PIPE_P1, rxData, sizeof(rxData));
            pc.printf("READ %d BYTES OF DATA.\r\n", rx_bytes);            
            wifi_write(rxData, rx_bytes);
        }
    }
}
