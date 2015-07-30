// ShipIoT.h
//
// © 2015 wot.io Inc. <devs@wot.io>
//

#ifndef HAVE_SHIPIOT
#define HAVE_SHIPIOT


class ShipIoT {
	public:
		ShipIoT(char* url,char* user,char* password);
		void setup(char* ssid, char* password);		
		void send(String &message);
		bool available();
		char read();
	
	private:	
		bool connected;
		char* server;
		char* path;
		char* user;
		char* password;
		char* token;
};

extern ShipIoT shipiot;

#endif
