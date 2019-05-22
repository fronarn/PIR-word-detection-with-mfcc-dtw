#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#pragma comment(lib, "Ws2_32.lib")

#include "Sensor.h"
#include <string>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include "Ws2tcpip.h"
#include "pugixml.hpp"

using namespace std;
using namespace pugi;


Sensor::Sensor(string appI)
{
	appId = appI;
}


Sensor::~Sensor()
{
}

void Sensor::getContent() {


	// const char* host = HOST;
	// const string name = "http://" + (string)HOST + ":8080/~/in-cse/in-name/" + appId + "/DATA/la";192.168.43.120
	const char* host = "192.168.43.120";
	const string hname = "http://192.168.43.120:8080/~/in-cse/in-name/" + appId + "/DATA/la";
	int port = 8080;

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup failed.\n";
		exit(1);
	}

	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons((unsigned short)port);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr	= inet_addr(host);
	// InetPton(AF_INET, host, &SockAddr.sin_addr.s_addr); COMMENTE A CAUSE DE BUG

	cout << "Connecting...\n";

	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		cout << "Could not connect";
		exit(1);
	}
	cout << "Connected.\n";

	// Build request
	string req = "GET"; // GET | POST
	req.append(" ");
	// Note, on GET, 'resource' must contain the encoded parameters, if any:
	req.append("/~/in-cse/in-name/" + appId + "/DATA/la");
	req.append(" HTTP/1.1\r\n");

	req.append("Host: ");
	req.append(hname);
	req.append("\r\n");

	req.append("X-M2M-Origin: admin:admin\r\n");
	req.append("Accept: application/xml\r\n\r\n");


	cout << "=============================== request"
		<< endl
		<< endl
		<< req
		<< "=============================== "
		<< endl;

	send(Socket, req.c_str(), (int)req.size(), 0);

	char buffer[2048]; // hope message will never be longer
	int nlen;

	nlen = recv(Socket, buffer, 1024, 0);

	closesocket(Socket);
	WSACleanup();

	if (nlen > 2048) cout << "erreur message trop long" << endl;

	buffer[nlen] = '\0';

	string content = buffer;

	replaceString(content, "&lt;", "<");
	replaceString(content, "&quot;", "\"");

	cout << content << endl;

	xml_document doc;
	doc.load_string(content.c_str());


	for (auto&& obj : doc.first_child().child("con").child("obj"))
	{

		if (obj.attribute("name").as_string() == (string)"appId") appId = obj.attribute("val").as_string();
		if (obj.attribute("name").as_string() == (string)"name") name = obj.attribute("val").as_string();
		if (obj.attribute("name").as_string() == (string)"data")  data = obj.attribute("val").as_float();
		if (obj.attribute("name").as_string() == (string)"location") location = obj.attribute("val").as_string();
		if (obj.attribute("name").as_string() == (string)"category") category = obj.attribute("val").as_string();

	}

}

void Sensor::updateValue(string value) {

	const char* host = "192.168.43.120";
	const string hname = "http://192.168.43.120:8080/~/in-cse/in-name/" + appId + "/DATA/la";
	int port = 8080;

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup failed.\n";
		exit(1);
	}

	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons((unsigned short)port);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = inet_addr(host);
	// InetPton(AF_INET, host, &SockAddr.sin_addr.s_addr); COMMENTE A CAUSE DE BUG

	cout << "Connecting...\n";

	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		cout << "Could not connect";
		exit(1);
	}
	cout << "Connected.\n";


	string message1 = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\">\r\n"
		"    <cnf>message</cnf>\r\n"
		"	<con>\r\n\r\n"
		"	&lt;obj&gt;\r\n\r\n"
		"		&lt;str name=&quot;appId&quot; val=&quot;";
	string message2 = "&quot;/&gt;\r\n\r\n"
		"		&lt;str name=&quot;name&quot; val=&quot;";
	string message3 = "&quot;/&gt;\r\n\r\n"
		"		&lt;int name=&quot;data&quot; val=&quot;";
	string message4 = "&quot;/&gt;\r\n\r\n"
		"		&lt;int name=&quot;location&quot; val=&quot;";
	string message5 = "&quot;/&gt;\r\n\r\n"
		"		&lt;int name=&quot;category&quot; val=&quot;";
	string message6 = "&quot;/&gt;\r\n\r\n"
		"	&lt;/obj&gt;\r\n\r\n"
		"</con>\r\n"
		"</m2m:cin>\r\n";


	string message = message1 + appId + message2 + name + message3 + value + message4 + location + message5 + category + message6;

	// Build request
	string req = "POST"; // GET | POST
	req.append(" ");
	// Note, on GET, 'resource' must contain the encoded parameters, if any:
	req.append("/~/in-cse/in-name/" + appId + "/DATA");
	req.append(" HTTP/1.1\r\n");

	req.append("Host: ");
	req.append(hname);
	req.append("\r\n");
	req.append("Content-length: ");
	req.append(to_string(message.size()));
	req.append("\r\n");
	req.append("X-M2M-Origin: admin:admin\r\n");
	req.append("Content-type: application/xml;ty=4\r\n\r\n");
	req.append(message);


	cout << "=============================== request"
		<< endl
		<< req
		<< "=============================== "
		<< endl;

	send(Socket, req.c_str(), (int)req.size(), 0);

	char buffer[2048]; // hope message will never be longer
	int nlen;

	nlen = recv(Socket, buffer, 1024, 0);

	if (nlen > 2048) cout << "erreur message trop long" << endl;

	buffer[nlen] = '\0';

	cout << buffer << endl;
	closesocket(Socket);
	WSACleanup();

}
int Sensor::waitEvent() {
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	// Accept a client socket
	cout << "waiting for message" << endl;
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}



	// Receive until the peer shuts down the connection

	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (iResult >= 0) {
		printf("Bytes received: %d\n", iResult);
		recvbuf[iResult] = '\0';

		string content = recvbuf;

		replaceString(content, "&lt;", "<");
		replaceString(content, "&quot;", "\"");

		cout << content << endl;

		xml_document doc;
		doc.load_string(content.c_str());


		for (auto&& obj : doc.first_child().child("nev").first_child().first_child().child("con").child("obj"))
		{
			if (obj.attribute("name").as_string() == (string)"appId") appId = obj.attribute("val").as_string();
			if (obj.attribute("name").as_string() == (string)"name") name = obj.attribute("val").as_string();
			if (obj.attribute("name").as_string() == (string)"data") data = obj.attribute("val").as_float();
			if (obj.attribute("name").as_string() == (string)"location") location = obj.attribute("val").as_string();
			if (obj.attribute("name").as_string() == (string)"category") category = obj.attribute("val").as_string();

		}

		cout << "Sending result" << endl;

		iSendResult = send(ClientSocket, "HTTP/1.1 200\r\nContent-type: application/xml\r\nContent-length: 2\r\nX-M2M-Origin: admin:admin\r\n\r\nOK", 96, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	}

	else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}


	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ClientSocket);

	// cleanup
	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}

//
//int Sensor::createSubscription(string sub_name, string host) {
//
//	const char* host = "127.0.0.1";
//	const string name = "http://127.0.0.1:8080/~/in-cse/in-name/" + appId + "/DATA/la";
//	int port = 8080;
//
//	WSADATA wsaData;
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		cout << "WSAStartup failed.\n";
//		exit(1);
//	}
//
//	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//
//
//	SOCKADDR_IN SockAddr;
//	SockAddr.sin_port = htons((unsigned short)port);
//	SockAddr.sin_family = AF_INET;
//	InetPton(AF_INET, host, &SockAddr.sin_addr.s_addr);
//
//	cout << "Connecting...\n";
//
//	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
//	{
//		cout << "Could not connect";
//		exit(1);
//	}
//	cout << "Connected.\n";
//
//
//	string message1 = "<m2m:sub xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"";
//	string message2 = "\">\r\n	<nu>http://" > ;
//	string message3 = "</nu>\r\n	<nct>1</nct>\r\n</m2m:sub>";
//
//	string message = message1 + sub_name +message2 + host +message3;
//
//
//	// Build request
//	string req = "POST"; // GET | POST
//	req.append(" ");
//	// Note, on GET, 'resource' must contain the encoded parameters, if any:
//	req.append("/~/in-cse/in-name/" + appId + "/DATA");
//	req.append(" HTTP/1.1\r\n");
//
//	req.append("Host: ");
//	req.append(name);
//	req.append("\r\n");
//	req.append("Content-length: ");
//	req.append(to_string(message.size()));
//	req.append("\r\n");
//	req.append("X-M2M-Origin: admin:admin\r\n");
//	req.append("Content-type: application/xml;ty=4\r\n\r\n");
//	req.append(message);
//
//
//	cout << "=============================== request"
//		<< endl
//		<< req
//		<< "=============================== "
//		<< endl;
//
//	send(Socket, req.c_str(), (int)req.size(), 0);
//
//	char buffer[2048]; // hope message will never be longer
//	int nlen;
//
//	nlen = recv(Socket, buffer, 1024, 0);
//
//	if (nlen > 2048) cout << "erreur message trop long" << endl;
//
//	buffer[nlen] = '\0';
//
//	cout << buffer << endl;
//	closesocket(Socket);
//	WSACleanup();
//
//	return 1;
//}



void Sensor::replaceString(std::string& subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

