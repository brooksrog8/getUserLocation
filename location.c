#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")

void getGeolocation() {
    HINTERNET hInternet = InternetOpen("GeolocationApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("Failed to open internet connection.\n");
        return;
    }

    HINTERNET hConnect = InternetOpenUrl(hInternet, "https://www.geoplugin.net/json.gp", NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("Failed to connect to geolocation service.\n");
        InternetCloseHandle(hInternet);
        return;
    }

    char buffer[1024];
    DWORD bytesRead;
    BOOL bResult = InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead);
    if (bResult && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Response: %s\n", buffer);

        char *latitude = strstr(buffer, "\"geoplugin_latitude\":");
        char *longitude = strstr(buffer, "\"geoplugin_longitude\":");
        char *city = strstr(buffer, "\"geoplugin_city\":");
        char *region = strstr(buffer, "\"geoplugin_region\":");
        char *country = strstr(buffer, "\"geoplugin_countryName\":");
        char *timezone = strstr(buffer, "\"geoplugin_timezone\":");
        
        if (latitude && longitude && city && region && country && timezone) {
            latitude += 21;
            longitude += 22;
            city += 18;    
            region += 20;   
            country += 24;   
            timezone += 22;  

            char lat[20], lon[20], ct[100], rg[100], cntry[100], tz[100];
            strcpy(lat, strtok(latitude, ","));
            strcpy(lon, strtok(longitude, ","));
            strcpy(ct, strtok(city, ","));
            strcpy(rg, strtok(region, ","));
            strcpy(cntry, strtok(country, ","));
            strcpy(tz, strtok(timezone, ","));

            FILE *file = fopen("geolocation.txt", "w");
            if (file != NULL) {
                fprintf(file, "Latitude: %s\n", lat);
                fprintf(file, "Longitude: %s\n", lon);
                fprintf(file, "City: %s\n", ct);
                fprintf(file, "Region: %s\n", rg);
                fprintf(file, "Country: %s\n", cntry);
                fprintf(file, "Timezone: %s\n", tz);
                fclose(file);
                printf("Geolocation data written to geolocation.txt\n");
            } else {
                printf("Failed to open file for writing.\n");
            }
        }
    } else {
        printf("Failed to read response from geolocation service.\n");
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    getGeolocation();

    WSACleanup();
    return 0;
}
