#include "Listener.h"
#include "Session.h"

#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>

#include <iostream>
#include <memory>


//----------------------------------------------------------------------------------------------------------------------
Listener::Listener(boost::asio::io_context& ioContext,
                   boost::asio::ip::tcp::endpoint endpoint)
    :
    m_ioContext(ioContext),
    m_acceptor(ioContext)
{
    boost::beast::error_code errorCode;

    // Open the acceptor
    m_acceptor.open(endpoint.protocol(), errorCode);
    if (errorCode)
    {
        std::ostringstream msg("Failed to open acceptor. Error code: ");
        msg << errorCode;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }

    // Allow address reuse
    m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), errorCode);
    if (errorCode)
    {
        std::ostringstream msg("Failed to set reuse option on socket. Error code: ");
        msg << errorCode;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }

    // Bind to the server address
    m_acceptor.bind(endpoint, errorCode);
    if (errorCode)
    {
        std::ostringstream msg("Failed to bind socket to endpoint. Error code: ");
        msg << errorCode;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }

    // Start listening for connections
    m_acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);
    if (errorCode)
    {
        std::ostringstream msg("Failed to start listenting. Error code: ");
        msg << errorCode;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Listener::run()
{
    doAccept();
}

//----------------------------------------------------------------------------------------------------------------------
void Listener::doAccept()
{
    // Post an accept on its own strand
    m_acceptor.async_accept(boost::asio::make_strand(m_ioContext),
                            boost::beast::bind_front_handler(&Listener::onAccept, shared_from_this()));
}

//----------------------------------------------------------------------------------------------------------------------
void Listener::onAccept(boost::beast::error_code errorCode, boost::asio::ip::tcp::socket socket)
{
    if (errorCode)
    {
        // TODO - Fairly certain we cannot throw from the strand
        //        Would probably just log and continue on to accept other connections
        std::cerr << "OnAccept handler called with error code: " << errorCode << std::endl;
    }
    else
    {
        // Create the session and run it
        std::make_shared<Session>(std::move(socket))->run();
    }

    // Post another accept
    doAccept();
}

