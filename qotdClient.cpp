#include "qotd.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;
#pragma comment(lib, "ws2_32.lib")

int main() {
	char server_name[MAX_SERVER_NAME] = "";
	cout << "QOTD Server? ";
	cin.getline(server_name, 80);

	char port[MAX_PORT];
	strcpy_s(port, "qotd");

	//Must always initialize the Windows Sockets tcp/ip Library
	WORD wVersionRequired = 0x0202;
	WSADATA wsaData;

	int err = WSAStartup(wVersionRequired, &wsaData);
	if (err) {
		cout << "Unable to initialize Windows Socket Library." << endl;
		return 0;
	}

	//call function to create udp socket
	SOCKET s = connectsock("", "", "udp");
	if (s != INVALID_SOCKET) {
		char sendCommand[MAX_SEND_BUFFER] = "";
		cout << "Command to send> ";
		cin.getline(sendCommand, MAX_SEND_BUFFER - 1);
		int messageLength = (int)strlen(sendCommand);

		while (_stricmp(sendCommand, "QUIT") != 0 && messageLength>0) {
			int len = UDP_send(s, sendCommand, messageLength + 1, server_name, port);
			if (len > 0) {
				cout << endl << "Quote of the day: " << endl;
				len = wait(s, 1, 0);
				if (len > 0) {
					char receiveBuffer[MAX_RECV_BUFFER] = "";
					len = UDP_recv(s, receiveBuffer, MAX_RECV_BUFFER - 1, server_name, port);
					while (len > 0) {

						receiveBuffer[len] = '\0';
						cout << receiveBuffer;

						len = wait(s, 1, 0);
						if(len > 0) len = UDP_recv(s, receiveBuffer, MAX_RECV_BUFFER - 1, server_name, port);
					}	
				}
				cout << endl;
			}
			cout << "Command to send? ";
		}
	}
}