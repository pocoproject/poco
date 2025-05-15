#ifndef Net_HTTPSocketAcceptor_INCLUDED
#define Net_HTTPSocketAcceptor_INCLUDED

#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/SocketReactor.h"

namespace Poco {
    namespace Net {
        template<typename HTTPServiceHandler>
        class HTTPSocketAcceptor: public SocketAcceptor<HTTPServiceHandler>
        {
        public:
            HTTPSocketAcceptor(ServerSocket& socket, SocketReactor& reactor, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory):
                SocketAcceptor<HTTPServiceHandler>(socket, reactor),
                _pParams(pParams),
                _pFactory(pFactory),
                _reactor(reactor)
            {
            }

            ~HTTPSocketAcceptor()
            {
            }

        private:
            HTTPServiceHandler* createServiceHandler(StreamSocket& socket) override
            {
                auto pHandler = new HTTPServiceHandler(socket, _reactor);
                pHandler->setParams(_pParams);
                pHandler->setFactory(_pFactory);
                return pHandler;
            }
            HTTPServerParams::Ptr _pParams;
            HTTPRequestHandlerFactory::Ptr _pFactory;
            SocketReactor& _reactor;
        };
    }
}


#endif