// usb-data-read-write.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h> // This is required to include before setupapi.h
#include <setupapi.h>
#include <iostream>
#include <vector>
#include <initguid.h>
#include <Usbiodef.h>
#include <Usb100.h>
#include <Usbioctl.h>
#include <string>

std::vector<std::string> GetDevicePaths(GUID* guid)
{
	std::vector<std::string> device_paths;
	HDEVINFO device_information = SetupDiGetClassDevs(guid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
	DWORD member_index = 0;
	DWORD last_error = NO_ERROR;
	SP_DEVINFO_DATA device_information_data;;
	SP_DEVICE_INTERFACE_DATA  device_interface_data;
	PSP_DEVICE_INTERFACE_DETAIL_DATA  device_interface_detail_data;

	if (INVALID_HANDLE_VALUE != device_information)
	{
		while (ERROR_NO_MORE_ITEMS != GetLastError())
		{
			device_information_data.cbSize = sizeof(SP_DEVINFO_DATA);

			if (SetupDiEnumDeviceInfo(device_information, member_index, &device_information_data))
			{
				device_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

				if (SetupDiEnumDeviceInterfaces(device_information, NULL, guid, member_index, &device_interface_data))
				{
					DWORD required_size = 0;
					SetupDiGetDeviceInterfaceDetail(device_information, &device_interface_data, NULL, 0, &required_size, NULL);
					device_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(required_size);
					device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
					if (SetupDiGetDeviceInterfaceDetail(device_information, &device_interface_data, device_interface_detail_data, required_size, NULL, NULL))
					{
						std::wstring wstring(device_interface_detail_data->DevicePath);
						std::string device_path(wstring.begin(), wstring.end());
						device_paths.push_back(device_path);
					}
					else
					{
						last_error = GetLastError();
					}
					if (NULL != device_interface_detail_data)
					{
						free(device_interface_detail_data);
					}
				}
				else
				{
					last_error = GetLastError();
				}
			}
			else
			{
				last_error = GetLastError();
			}
			member_index++;
		}
	}
	else
	{
		last_error = GetLastError();
	}
	return device_paths;
}




int main()
{
	std::vector<std::string> device_paths = GetDevicePaths((LPGUID)&GUID_DEVINTERFACE_USB_DEVICE);
	for (auto device_path : device_paths)
	{
		std::cout << "---------------------------------------------------- \n";
		std::cout << "Device Path = " << device_path << "\n";
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
