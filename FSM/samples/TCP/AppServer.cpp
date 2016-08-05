//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy
// of the License at http://www.mozilla.org/MPL/
// 
// Software distributed under the License is distributed on an
// "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
// 
// The Original Code is State Machine Compiler (SMC).
// 
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2007. Charles W. Rapp.
// All Rights Reserved.
// 
// Contributor(s): 
//
// Name
//  AppServer.cpp
//
// Description
//  Encapsulates a TcpServer object.
//
// RCS ID
// $Id: AppServer.cpp,v 1.5 2007/12/28 12:34:40 cwrapp Exp $
//
// CHANGE LOG
// $Log: AppServer.cpp,v $
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:35:08  charlesr
// Initial revision
//

#include "AppServer.h"
#include "AppClient.h"
#include "Eventloop.h"
#if defined(WIN32)
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <string>

using namespace std;

// Externally defined global variables.
extern Eventloop *Gevent_loop;

// Constants.
const static char *DELETE_TIMER = "DeleteTimer";

//---------------------------------------------------------------
// AppServer() (Public)
// Default constructor.
//
AppServer::AppServer()
: _server(NULL),
  _clientList(NULL),
  _deleteList(NULL)
{}

//---------------------------------------------------------------
// ~AppServer() (Public)
// Destructor. Close and delete all accepted client connections.
//
AppServer::~AppServer()
{
    ClientEntry *client,
                *nextClient;

    // Close and delete the server if necessary.
    if (_server != NULL)
    {
        _server->doClose();
        delete _server;
        _server = NULL;
    }

    for (client = _clientList;
         client != NULL;
         client = nextClient)
    {
        nextClient = client->getNext();
        (client->getClient())->close();
        delete client;
    }
    _clientList = NULL;

    for (client = _deleteList;
         client != NULL;
         client = nextClient)
    {
        nextClient = client->getNext();
        delete client;
    }
    _deleteList = NULL;

    return;
} // end of AppServer::~AppServer()

//---------------------------------------------------------------
// open(unsigned short) (Public)
// Create a TCP service object and have it start opening.
//
void AppServer::open(unsigned short port)
{
    if (_server == NULL)
    {
        _server = new TcpServer(*this);
        _server->open(port);
    }

    return;
} // end of AppServer::open(unsigned short)

//---------------------------------------------------------------
// close() (Public)
// Have the TCP service start the close process.
//
void AppServer::close()
{
    if (_server != NULL)
    {
        _server->doClose();
    }

    return;
} // end of AppServer::close()

//---------------------------------------------------------------
// clientClosed(const AppClient&) (Public)
// One of the accepted client connections is now closed. Remove
// it from the client list.
//
void AppServer::clientClosed(const AppClient& client)
{
    ClientEntry **entry,
                 *remove_entry;

    for (entry = &_clientList;
         *entry != NULL;
         entry = &((*entry)->_next))
    {
        if ((*entry)->getClient() == &client)
        {
            remove_entry = *entry;
            *entry = (*entry)->getNext();

            // Now put the client on the delete list and
            // start the delete timer.
            remove_entry->setNext(_deleteList);
            _deleteList = remove_entry;

            if (Gevent_loop->doesTimerExist(DELETE_TIMER) == true)
            {
                Gevent_loop->startTimer(DELETE_TIMER, 1, *this);
            }

            break;
        }
    }

    return;
} // end of AppServer::clientClosed(const AppClient&)

//---------------------------------------------------------------
// opened(TcpConnection&) (Public)
// TCP service successfully opened.
//
void AppServer::opened(TcpConnection&)
{
    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = _server->getPort();
    address.sin_addr.s_addr = _server->getAddress();

    cout << "Service opened on "
         << inet_ntoa(address.sin_addr)
         << ":"
         << ntohs(address.sin_port)
         << "."
         << endl;

    return;
} // end of AppServer::opened(TcpConnection&)

//---------------------------------------------------------------
// openFailed(const char*, TcpConnection&) (Public)
// TCP service failed to open.
//
void AppServer::openFailed(const char *reason, TcpConnection&)
{
    void socketClosed();

    cout << "Service failed to open on port "
         << _server->getPort()
         << ", reason: "
         << reason
         << endl;

    // Tell the application that the TCP service is closed.
    socketClosed();

    return;
} // end of AppServer::openFailed(const char*, TcpConnection&)

//---------------------------------------------------------------
// closed(const char*, TcpConnection&) (Public)
// TCP service is now closed.
//
void AppServer::closed(const char *reason, TcpConnection&)
{
    void socketClosed();

    cout << "Service closed: " 
         << reason
         << endl;

    // Tell the application that the TCP service is closed.
    socketClosed();

    return;
} // end of AppServer::closed(TcpConnection&)

//---------------------------------------------------------------
// accepteded(TcpClient&, TcpServer&) (Public)
// TCP service has accepted a new connection.
//
void AppServer::accepted(TcpClient& client, TcpServer&)
{
    AppClient *new_client;
    ClientEntry *new_entry;
    const sockaddr_in& address = client.getServerAddress();
    string host;

    host = inet_ntoa(address.sin_addr);

    cout << "Accepted new connection from "
         << host
         << ":"
         << ntohs(address.sin_port)
         << "."
         << endl;

    // Create a new client handler and put in the client list.
    new_client = new AppClient(host, client, *this);
    new_entry = new ClientEntry(*new_client);
    new_entry->setNext(_clientList);
    _clientList = new_entry;

    return;
} // end of AppServer::accepted(TcpClient&, TcpServer&)

//---------------------------------------------------------------
// handleTimeout(const char*) (Public)
// Time to delete the zombie clients.
//
void AppServer::handleTimeout(const char *name)
{
    if (strcmp(name, DELETE_TIMER) == 0)
    {
        ClientEntry *it;
        ClientEntry *next;

        for (it = _deleteList; it != NULL; it = next)
        {
            next = it->getNext();
            it->setNext(NULL);
            delete it;
        }

        _deleteList = NULL;
    }

    return;
} // end of AppServer::handleTimeout(const char*)
