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
#include "proton/Session.hpp"
#include "contexts.hpp"

#include "proton/connection.h"
#include "proton/session.h"
#include "proton/Session.hpp"
#include "proton/Connection.hpp"
#include "ConnectionImpl.hpp"

namespace proton {
namespace reactor {

template class ProtonHandle<pn_session_t>;
typedef ProtonImplRef<Session> PI;

Session::Session(pn_session_t *p) {
    PI::ctor(*this, p);
}
Session::Session() {
    PI::ctor(*this, 0);
}
Session::Session(const Session& c) : ProtonHandle<pn_session_t>() {
    PI::copy(*this, c);
}
Session& Session::operator=(const Session& c) {
    return PI::assign(*this, c);
}
Session::~Session() {
    PI::dtor(*this);
}

pn_session_t *Session::getPnSession() { return impl; }

void Session::open() {
    pn_session_open(impl);
}

Connection &Session::getConnection() {
    pn_connection_t *c = pn_session_connection(impl);
    return ConnectionImpl::getReactorReference(c);
}

Receiver Session::createReceiver(std::string name) {
    pn_link_t *link = pn_receiver(impl, name.c_str());
    return Receiver(link);
}

Sender Session::createSender(std::string name) {
    pn_link_t *link = pn_sender(impl, name.c_str());
    return Sender(link);
}

}} // namespace proton::reactor
