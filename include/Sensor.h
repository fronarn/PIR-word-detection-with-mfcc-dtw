#ifndef _SENSOR_
#define _SENSOR_



#pragma once
#include <iostream>
#include <string>

using namespace std;

#define DEFAULT_BUFLEN 2048
#define DEFAULT_PORT "2345"
#define HOST ("192.168.43.120")

class Sensor
{
public:

private:
	string appId;
	string name;
	float data;
	string location;
	string category;


public:

	Sensor(string appI);
	virtual ~Sensor();

	// accesseurs :

	string getAppId() {
		return appId;
	}

	void setAppId(string s) {
		appId = s;
	}

	string getName() {
		return name;
	}

	void setName(string s) {
		name = s;
	}

	float getData() {
		return data;
	}

	void setData(float d) {
		data = d;
	}

	string getLocation() {
		return location;
	}

	void setLocation(string s) {
		location = s;
	}


	// fonction pour actualiser les donn�es
	void getContent();

	// fonction pour envoyer une nouvelle donn�e
	void updateValue(string value);

	//fonction pour attendre les donn�es d'un abonnement
	// fonction bloquante qui retourne la nouvelle valeur du capteur 
	int waitEvent();

	//int createSubscription(string sub_name, string host);

	// fonction interne
	void replaceString(std::string& subject, const std::string& search, const std::string& replace);

};

#endif // !_SENSOR_
