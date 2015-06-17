/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include "proton/Connection.hpp"
#include "proton/Transport.hpp"
#include "proton/Container.hpp"
#include "proton/Event.hpp"
#include "proton/connection.h"
#include "Connector.hpp"
#include "ConnectionImpl.hpp"
#include "Url.hpp"

namespace proton {
namespace reactor {

Connector::Connector(Connection &c) : connection(c), transport(0) {}

Connector::~Connector() {}

void Connector::setAddress(const std::string &a) {
    address = a;
}

void Connector::connect() {
    pn_connection_t *conn = connection.getPnConnection();
    pn_connection_set_container(conn, connection.getContainer().getContainerId().c_str());
    Url url(address);
    std::string hostname = url.getHost() + ":" + url.getPort();
    pn_connection_set_hostname(conn, hostname.c_str());
    transport = new Transport();
    transport->bind(connection);
    connection.impl->transport = transport;
}


void Connector::onConnectionLocalOpen(Event &e) {
    connect();
}

void Connector::onConnectionRemoteOpen(Event &e) {}

void Connector::onConnectionInit(Event &e) {
}

void Connector::onTransportClosed(Event &e) {
    // TODO: prepend with reconnect logic
    pn_connection_release(connection.impl->pnConnection);
    // No more interaction, so drop our counted reference.
    connection = Connection();
}


}} // namespace proton::reactor
