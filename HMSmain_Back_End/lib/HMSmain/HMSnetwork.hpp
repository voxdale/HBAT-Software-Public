/*
 Network.h - HBAT library
 Copyright (c) 2021 Zacariah Austin Heim.
*/
#pragma once
#ifndef HMSNETWORK_hpp
#define HMSNETWORK_hpp

#include "defines.hpp"
#include <ESPmDNS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <strTools.h>


class HMSnetwork
{
public:
    // constructors
    HMSnetwork();
    virtual ~HMSnetwork();
    // Functions
    bool SetupNetworkStack();
    int CheckWifiState();
    bool connectToApWithFailToStation();
    void SetupWebServer();
    int DiscovermDNSBroker();
    void SetupmDNSServer();
    bool SetupmDNSLoop();
    void loadConfig();
    void SetupServer();
    void CheckNetworkLoop();
    void writeFile(fs::FS &fs, const char * path, const char * message);
    String readFile(fs::FS &fs, const char *path);
    //String processor(const String& var);

    // variables
private:
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_NETWORK)
extern HMSnetwork network;
#endif

#endif
