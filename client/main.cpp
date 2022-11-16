#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <exception>
#include <vector>
#include <memory>
#include <ctime>
#include "DeviceINQ.h"
#include "Enums.h"
#include "BluetoothException.h"
#include "BTSerialPortBinding.h"

using namespace std;

string formatDate(const char *format, time_t time)
{
	if (time <= 0)
		return "--";

	char buffer[256] = { 0 };
	tm *timeinfo = localtime(&time);

	if (timeinfo)
		strftime(buffer, sizeof(buffer), format, timeinfo);

	return buffer;
}

int main()
{
	try
	{
		unique_ptr<DeviceINQ> inq(DeviceINQ::Create());
		vector<device> devices = inq->Inquire();
		int id =0;
		for (const auto& d : devices)
		{	cout << "\tdevice_id: "<<id<<endl;
			cout << "\tname: " << d.name << endl;
			cout << "\taddress: " << d.address << endl;
			cout << endl;
			id++;
		}
		int device_id;
		cout << "\nSelect device_id: ";
		cin>>device_id;
		BTSerialPortBinding *x = BTSerialPortBinding::Create(devices[device_id].address,inq->SdpSearch(devices[device_id].address));
		x->Connect();
		cout<<x->IsDataAvailable();
		char buf[]="hello lco";
		x->Write(buf,6);
		cout << endl << "done, found " << devices.size() << " device(s)" << endl;
		x->Read(buf,12);
		cout<<"rev: "<<buf;
	}
	catch (const BluetoothException& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

