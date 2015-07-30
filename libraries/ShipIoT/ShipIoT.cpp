// ShipIoT.cpp
//
// © 2015 wot.io Inc. <devs@wot.io>
//
//
//


#ifndef __CC3200R1M1RGC__ 
// the CC3200 doesn't use SPI
 #include <SPI.h>
#endif
#include <WiFi.h>
#include <ShipIoT.h>

WiFiClient client;

char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char* encode(const char* text) {
	int i = 0, j = 0;
	char* buffer = new char[strlen(text) + 4];
	char* retval = new char[((strlen(text)+2) / 3) * 4 + 1];
	memset(retval,0,((strlen(text)+2) / 3) * 4 + 1);
	memset(buffer,0,strlen(text));
	memcpy(buffer, text, strlen(text));
	for (i = 0; i < strlen(text); i += 3) {
		retval[j++] = b64[(buffer[i] & 0xfc) >> 2];
		retval[j++] = b64[((buffer[i] & 0x03) << 4) + ((buffer[i+1] & 0xf0) >> 4)] ;
		retval[j++] = b64[((buffer[i+1] & 0x0f) << 2) + ((buffer[i+2] & 0xc0) >> 6)] ;
		retval[j++] = b64[(buffer[i+2] & 0x3f)];
	}
	retval[j] = 0;
	for (i = strlen(retval) - 1; retval[i] == 'A'; --i) retval[i] = '=';	// trim trailing nulls
	delete buffer;
	return retval;
}

void ShipIoT::setup(char* ssid, char* password) {
	WiFi.begin(ssid,password);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(300);
	};
	Serial.println();
	while (WiFi.localIP() == INADDR_NONE) {
		Serial.print(".");
		delay(300);
	}
	Serial.println();
}

ShipIoT::ShipIoT(char* Url, char* User, char* Password): user(User), password(Password)  {
	char* buffer = new char[ strlen(user) + strlen(password) + 2 ]; 
	int i = 0, j = 0;
	for (i = 0; Url[i] != '/' && Url[i+1] != '/'; ++i);	// find ://
	i += 3; 						// skip ://
	for (j = i; Url[j] != '/'; ++j); 			// find first /	
	server = new char[ j - i + 1 ];
	memcpy(server,&Url[i],j-i);
	server[j-i] = 0;
	path = new char[ strlen(&Url[j]) + 1];
	memcpy(path,&Url[j],strlen(&Url[j]));
	path[strlen(&Url[j])] = 0;
	memcpy(buffer,user,strlen(user));
	buffer[ strlen(user) ] = ':';
	memcpy(buffer + strlen(user) + 1, password, strlen(password));
	buffer[ strlen(user) + strlen(password) + 1 ] = 0;
	token = encode(buffer);
	delete buffer;
}


void ShipIoT::send(String &message) {
	if (client.connect(server,80)) {
		Serial.println("Connected to server");
		String message_length = String(message.length());
		client.print("POST ");
		Serial.print("POST ");
		client.print(path);
		Serial.print(path);
		client.println(" HTTP/1.1");
		Serial.println(" HTTP/1.1");
		client.print("Host: ");
		Serial.print("Host: ");
		client.println(server);
		Serial.println(server);
		client.println("Connection: close");
		Serial.println("Connection: close");
		client.println("Content-Type: application/json");
		Serial.println("Content-Type: application/json");
		client.print("Authorization: Basic ");
		Serial.print("Authorization: Basic ");
		client.println(token);
		Serial.println(token);
		client.print("Content-Length: ");
		Serial.print("Content-Length: ");
		client.println(message_length);
		Serial.println(message_length);
		client.println();
		Serial.println();
		client.println(message);
		Serial.println(message);
	} else {
		Serial.print("Failed to connect to server ");
		Serial.println(server);
	}
}

bool ShipIoT::available() {
	return client.available();
}

char ShipIoT::read() {
	return client.read();
}

