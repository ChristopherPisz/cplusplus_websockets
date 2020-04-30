#include "Listener.h"

#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>

#include <iostream>
#include <memory>


Listener::Listener(boost::asio::io_context& io_context,
                   boost::asio::ip::tcp::endpoint endpoint)
    :
    m_io_context(io_context),
    m_acceptor(io_context)
{
    boost::beast::error_code error_code;

    // Open the acceptor
    m_acceptor.open(endpoint.protocol(), error_code);
    if (error_code)
    {
        std::ostringstream msg("Failed to open acceptor. Error code: ");
        msg << error_code;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }

    // Allow address reuse
    m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), error_code);
    if (error_code)
    {
        std::ostringstream msg("Failed to set reuse option on socket. Error code: ");
        msg << error_code;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }

    // Bind to the server address
    m_acceptor.bind(endpoint, error_code);
    if (error_code)
    {
        std::ostringstream msg("Failed to bind socket to endpoint. Error code: ");
        msg << error_code;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }

    // Start listening for connections
    m_acceptor.listen(boost::asio::socket_base::max_listen_connections, error_code);
    if (error_code)
    {
        std::ostringstream msg("Failed to start listenting. Error code: ");
        msg << error_code;
        // TODO - Use your own custom exception type
        throw std::exception(msg.str().c_str());
    }
}

// Start accepting incoming connections
void Listener::run()
{
    doAccept();
}

void Listener::doAccept()
{
    // Post an accept on its own strand
    m_acceptor.async_accept(boost::asio::make_strand(m_io_context),
                            boost::beast::bind_front_handler(&Listener::onAccept, shared_from_this()));
}

void Listener::onAccept(boost::beast::error_code error_code, boost::asio::ip::tcp::socket socket)
{
    if (error_code)
    {
        // TODO - Fairly certain we cannot throw from the strand
        //        Would probably just log and continue on to accept other connections
        std::cerr << "OnAccept handler called with error code: " << error_code << std::endl;
    }
    else
    {
        // Create the session and run it
        //std::make_shared<Session>(std::move(socket))->run();
    }

    // Post another accept
    doAccept();
}

