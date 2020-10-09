#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wlanapi.h>

#include <iostream>

using namespace std;

constexpr auto desired_wlan_client_version = 2;

VOID WINAPI callback(PWLAN_NOTIFICATION_DATA data, PVOID context)
{
	cout << "Received WLAN notification, source = " << data->NotificationSource << ", code = " << data->NotificationCode << endl;

	if (data->NotificationSource == WLAN_NOTIFICATION_SOURCE_MSM && data->NotificationCode == 59)
	{
		cout << ">>> UNDOCUMENTED MSM NOTIFICATION CODE 59 RECEIVED!!! <<<" << endl;
	}
}

int main()
{
	DWORD negotiatedWlanClientVersion;
	HANDLE wlanClientHandle;

	auto error = WlanOpenHandle(desired_wlan_client_version, nullptr, &negotiatedWlanClientVersion, &wlanClientHandle);
	if (error != ERROR_SUCCESS)
	{
		cerr << "Failed to open WLAN client handle, error is " << error << endl;
		return 1;
	}

	DWORD previousNotificationSource;

	error = WlanRegisterNotification(wlanClientHandle, WLAN_NOTIFICATION_SOURCE_ALL, true, callback, nullptr, nullptr, &previousNotificationSource);
	if (error != ERROR_SUCCESS)
	{
		cerr << "Failed to register WLAN notification, error is " << error << endl;
		return 1;
	}

	cout << "Registered WLAN notification handler" << endl;
	system("pause");

	error = WlanCloseHandle(wlanClientHandle, nullptr);
	if (error != ERROR_SUCCESS)
	{
		cerr << "Failed to close WLAN client handle, error is " << error << endl;
		return 1;
	}

	cout << "DONE" << endl;

	return 0;
}
