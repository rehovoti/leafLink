#ifndef WIFISETUP_H
#define WIFISETUP_H

/*
setup Wifi credentials using
https://github.com/tzapu/WiFiManager
https://github.com/tzapu/WiFiManager/blob/master/examples/OnDemand/OnDemandConfigPortal/OnDemandConfigPortal.ino
*/


#define TRIGGER_PIN 0
#define TIMEOUT 120

void initWiFiSetup();
void wifiSetupTask(void* parameters);

#endif