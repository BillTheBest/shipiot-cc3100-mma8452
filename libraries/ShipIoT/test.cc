
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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

int main(int argc, char** argv) {
	printf("%s\n",encode("test:test\n"));
}
