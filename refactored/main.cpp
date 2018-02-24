#include "sb_context.h"
#include "sb_message.h"
#include "sb_message_l2.h"
#include "sb_l2_packet.h"
#include "bt_addr.h"
#include <ios>
#include <iostream>
#include <iomanip>

void dump(const std::vector<uint8_t>& msg) {
	int i = 0;
	for (uint8_t byte :  msg) {
		std::cout << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)byte << " ";
		++ i;
		if (i%8 == 0) std::cout << std::endl;
	}
	if (i%8 != 0) std::cout << std::endl;
}

int main(int argc, const char* argv[]) {
	BtAddr addr("00:80:25:21:19:CA");
	SbContext context(addr);

	context.connect();
	context.ping();

	SbMessage login1 = context.waitFor(Login1);
	context.updateMyAddr(login1);

	context.request();
	SbMessage response = context.waitFor(Response);
	double signalStrength = (response[22] * 100) / 0xff;
	std::cout << "Signal Strength: " << std::dec << signalStrength << "%" << std::endl;

	MessageBytes mySusyId((uint8_t)(rand()%0xff), (uint8_t)(rand()%0xff));
	MessageBytes mySerial((uint8_t)(rand()%0xff), (uint8_t)(rand()%0xff), (uint8_t)(rand()%0xff), (uint8_t)(rand()%0xff));
	
	// Send L2 Packet
	SbL2Packet login(mySusyId, mySerial, "0000");
	context.send(SbMessageL2(context.getMyAddr(), login));
	response = context.waitFor(L2Packet);

	// S 7E 54 00 2A $ADD2 FF FF FF FF FF FF 01 00 7E FF 03 60 65 0E A0 FF FF FF FF FF FF 00 01 $MYSUSYID $MYSERIAL 00 01 00 00 00 00 $CNT 80 0C 04 FD FF 07 00 00 00 84 03 00 00 $TIME 00 00 00 00 $PASSWORD $CRC 7E $END;
	// R 7E 69 00 17 $ADDR $END;
	// E $LOGIN $END;
}


/*
Received: 0x 200
0x7e 0x1f 0x00 0x61 0xca 0x19 0x21 0x25 
0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
0x02 0x00 0x00 0x04 0x70 0x00 0x02 0x00 
0x00 0x00 0x00 0x01 0x00 0x00 0x00 
Sending: 0x0200
0x7e 0x1f 0x00 0x61 0x00 0x00 0x00 0x00 
0x00 0x00 0xca 0x19 0x21 0x25 0x80 0x00 
0x02 0x00 0x00 0x04 0x70 0x00 0x02 0x00 
0x00 0x00 0x00 0x01 0x00 0x00 0x00 
Waiting for 0x0500
Received: 0x0a00
0x7e 0x1f 0x00 0x61 0xca 0x19 0x21 0x25 
0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
0x0a 0x00 0xca 0x19 0x21 0x25 0x80 0x00 
0x01 0xdf 0xbc 0xeb 0xeb 0x27 0xb8 
Waiting for 0x0500
Received: 0x0c00
0x7e 0x14 0x00 0x6a 0xca 0x19 0x21 0x25 
0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
0x0c 0x00 0x02 0x00 
Waiting for 0x0500
Received: 0x0500
0x7e 0x22 0x00 0x5c 0xca 0x19 0x21 0x25 
0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
0x05 0x00 0xca 0x19 0x21 0x25 0x80 0x00 
0x01 0x01 0xdf 0xbc 0xeb 0xeb 0x27 0xb8 
0x02 0x01 
Found 0x0500
MyAddr:
0xdf 0xbc 0xeb 0xeb 0x27 0xb8 

Sending: 0x0300
0x7e 0x14 0x00 0x6a 0x00 0x00 0x00 0x00 
0x00 0x00 0xca 0x19 0x21 0x25 0x80 0x00 
0x03 0x00 0x05 0x00 
Waiting for 0x0400
Received: 0x0400
0x7e 0x18 0x00 0x66 0xca 0x19 0x21 0x25 
0x80 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
0x04 0x00 0x05 0x00 0x00 0x00 0xb8 0x00 


Found 0x0400
Signal Strength: 72%
Sending: 0x0100
0x7e 0x4d 0x00 0x33 0xdf 0xbc 0xeb 0xeb 
0x27 0xb8 0xff 0xff 0xff 0xff 0xff 0xff 
0x01 0x00 0x7e 0xff 0x03 0x60 0x65 0xa0 
0xff 0xff 0xff 0xff 0xff 0xff 0x00 0x01 
0xa3 0x97 0xa2 0x55 0x53 0xbe 0x00 0x01 
0x00 0x00 0x00 0x00 0x00 0x80 0x0c 0x04 
0xfd 0xff 0x07 0x00 0x00 0x00 0x84 0x03 
0x00 0x00 0xc0 0x00 0x00 0x00 0x00 0xb8 
0xb8 0xb8 0xb8 0x88 0x88 0x88 0x88 0x88 
0x88 0x88 0x88 0x3e 0x7e 
Waiting for 0x0100
*/
