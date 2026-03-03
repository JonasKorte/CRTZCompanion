/*
 * Copyright (c) 2020 Andreas Pohl
 * Licensed under MIT (https://github.com/apohl79/audiogridder/blob/master/COPYING)
 *
 * Author: Andreas Pohl
 */

#ifndef PortForwarding_hpp
#define PortForwarding_hpp

namespace e47 {

/** Attempts to add UPnP/NAT-PMP port mappings so the server is reachable from the internet.
 *  Maps serverPort (TCP) and workerPortStart..workerPortEnd (TCP) to the local machine.
 *  No-op if AG_ENABLE_UPNP is not defined or mapping fails (e.g. no UPnP IGD). */
void tryMapPorts(int serverPort, int workerPortStart, int workerPortEnd);

}  // namespace e47

#endif  // PortForwarding_hpp
