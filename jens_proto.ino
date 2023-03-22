#include <FastCRC.h>
FastCRC16 CRC16;

#pragma pack(1)
struct layer3Master{
    char RSSI;
    //LSBYte
    char TxID[3];
    char TxID_time[2];
};

struct layer3Packet{
    char type;
    char ReceiverID_Low;
    char ReceiverID_Middle;
    char ReceiverID_High;
    char TransducerLevel;
    char QuiteTime;
    char N;
    layer3Master Master[25];
};

struct layer2Packet{
    const char start = 0x02;
    char length;
    //char data[255];
    layer3Packet data;
    uint16_t crc;
    const char stop = 0x03;
};
#pragma pack()

void sendPacket(layer2Packet packet){
    Serial.write(packet.start);
    Serial.write(( uint8_t )packet.length);
    for(int i = 0; i < packet.length;i++){
        if(((uint8_t*)&packet.data)[i] == 0x02){
            Serial.write(0x10);
            Serial.write(0x02);
        }
        else if(((uint8_t*)&packet.data)[i] == 0x03){
            Serial.write(0x10);
            Serial.write(0x03);
        }
        else if(((uint8_t*)&packet.data)[i] == 0x10){
            Serial.write(0x10);
            Serial.write(0x10);
        }else{
            Serial.write(((uint8_t*)&packet.data)[i]);
        }

    }
    Serial.write((uint8_t)packet.crc);
    Serial.write((uint8_t)packet.stop);

}

layer2Packet makeLayer2Packet(layer3Packet packet){
    layer2Packet returnPacket;
    returnPacket.length = (6*packet.N)+7;
    returnPacket.data = packet;
    returnPacket.crc = CRC16.ccitt(((uint8_t*)&packet), returnPacket.length);
    return returnPacket;
}

void setup(){
    Serial.begin(9600);
}




void loop(){
        layer3Packet notSoCoolPacket;
    notSoCoolPacket.type = 0x01;
    notSoCoolPacket.ReceiverID_Low = 0x02;
    notSoCoolPacket.ReceiverID_Middle = 0x03;
    notSoCoolPacket.ReceiverID_High = 0x04;
    notSoCoolPacket.TransducerLevel = 0x05;
    notSoCoolPacket.QuiteTime = 0x06;
    notSoCoolPacket.N = 0x03;
        for (int i = 0; i < 3; i++)
    {
        notSoCoolPacket.Master[i].RSSI = 0x01;
        notSoCoolPacket.Master[i].TxID[0] = 0x02;
        notSoCoolPacket.Master[i].TxID[1] = 0x02;
        notSoCoolPacket.Master[i].TxID[2] = 0x02;
        notSoCoolPacket.Master[i].TxID_time[0]=0x01;
        notSoCoolPacket.Master[i].TxID_time[0]=0x02;
    }


    layer2Packet MegaCoolPacket = makeLayer2Packet(notSoCoolPacket);
    sendPacket(MegaCoolPacket);
    delay(10);
    //Serial.writeln("Der er hul igennem ");
    delay(1000);
}




