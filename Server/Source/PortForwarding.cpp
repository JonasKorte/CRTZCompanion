/*
 * Copyright (c) 2020 Andreas Pohl
 * Licensed under MIT (https://github.com/apohl79/audiogridder/blob/master/COPYING)
 *
 * Author: Andreas Pohl
 */

#include "PortForwarding.hpp"
#include "Logger.hpp"

#ifdef AG_ENABLE_UPNP

#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>

#include <cstring>
#include <cstdio>

#define setLogTagStatic(t) \
    static LogTag __tag(t); \
    auto getLogTagSource = [] { return &__tag; }
#define logln(M) \
    do { \
        String __msg, __str; \
        __msg << M; \
        __str << "[" << getLogTagSource()->getLogTag() << "] " << __msg; \
        Logger::log(__str); \
    } while (0)

namespace e47 {

void tryMapPorts(int serverPort, int workerPortStart, int workerPortEnd) {
    setLogTagStatic("portfwd");
    int upnperr = 0;
    struct UPNPDev* devlist = upnpDiscover(2000, nullptr, nullptr, 0, 0, &upnperr);
    if (!devlist) {
        logln("UPnP discover failed (no IGD or not supported), port forwarding not available");
        return;
    }
    char lanAddress[64] = {0};
    UPNPUrls urls;
    IGDdatas data;
    memset(&urls, 0, sizeof(urls));
    memset(&data, 0, sizeof(data));
    int ok = UPNP_GetValidIGD(devlist, &urls, &data, lanAddress, sizeof(lanAddress));
    freeUPNPDevlist(devlist);
    if (ok != 1) {
        logln("UPnP: no valid IGD found");
        return;
    }
    auto portStr = [](int p) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", p);
        return String(buf);
    };
    int mapped = 0;
    auto addMapping = [&](int port) {
        int r = UPNP_AddPortMapping(
            urls.controlURL, data.first.servicetype,
            portStr(port).toRawUTF8(),   /* external port */
            portStr(port).toRawUTF8(),   /* internal port */
            lanAddress,                  /* internal client */
            "AudioGridder",              /* description */
            "TCP",                       /* protocol */
            nullptr,                     /* remote host */
            "0");                        /* lease duration (0 = indefinite) */
        if (r == UPNPCOMMAND_SUCCESS) {
            logln("UPnP: mapped TCP " << port << " -> " << lanAddress << ":" << port);
            mapped++;
        }
    };
    addMapping(serverPort);
    int workerEnd = (workerPortEnd < workerPortStart + 50) ? workerPortEnd : (workerPortStart + 50);
    for (int p = workerPortStart; p <= workerEnd; p++) {
        addMapping(p);
    }
    if (mapped > 0) {
        logln("UPnP: " << mapped << " port(s) mapped for remote access");
    }
    FreeUPNPUrls(&urls);
}

}  // namespace e47

#else  // !AG_ENABLE_UPNP

namespace e47 {

void tryMapPorts(int /*serverPort*/, int /*workerPortStart*/, int /*workerPortEnd*/) {}

}  // namespace e47

#endif  // AG_ENABLE_UPNP
