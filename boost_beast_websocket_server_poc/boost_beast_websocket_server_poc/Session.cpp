#include "Session.h"

#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <memory>


//----------------------------------------------------------------------------------------------------------------------
Session::Session(boost::asio::ip::tcp::socket&& socket)
    :
    m_websocket(std::move(socket))
{
}

//----------------------------------------------------------------------------------------------------------------------
void Session::run()
{
    // Execution needs to take place within a strand to perform the async operations on the I/O objects in this Session.
    boost::asio::dispatch(m_websocket.get_executor(),
                          boost::beast::bind_front_handler(&Session::onRun, shared_from_this()));
}

//----------------------------------------------------------------------------------------------------------------------
void Session::onRun()
{
    // Set suggested timeout settings for the websocket
    m_websocket.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    m_websocket.set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type& res)
        {
            res.set(boost::beast::http::field::server,
                std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-async");
        }));

    // Accept the websocket handshake
    m_websocket.async_accept(boost::beast::bind_front_handler(&Session::onAccept, shared_from_this()));
}

//----------------------------------------------------------------------------------------------------------------------
void Session::onAccept(boost::beast::error_code error_code)
{
    if (error_code)
    {
        // TODO - Fairly certain we cannot throw from the strand
        //        Would probably just log and continue on to accept other connections
        std::cerr << "Session::onAccept called with error code: " << error_code << std::endl;
    }

    // Read a message
    doRead();
}

//----------------------------------------------------------------------------------------------------------------------
void Session::doRead()
{
    // Read a message into our buffer
    m_websocket.async_read(m_buffer,
                           boost::beast::bind_front_handler(&Session::onRead, shared_from_this()));
}

//----------------------------------------------------------------------------------------------------------------------
void Session::onRead(boost::beast::error_code errorCode, std::size_t bytesTransferred)
{
    boost::ignore_unused(bytesTransferred);

    // This indicates that the Session was closed
    if (errorCode == boost::beast::websocket::error::closed)
    {
        return;
    }

    if (errorCode)
    {
        // TODO - Fairly certain we cannot throw from the strand
        //        Would probably just log
        //        Might flag the session for closure if repeated errors. Test.
        std::cerr << "Session::onRead called with error code: " << errorCode << std::endl;
    }

    // Echo the message
    m_websocket.text(m_websocket.got_text());
    m_websocket.async_write(m_buffer.data(),
                            boost::beast::bind_front_handler(&Session::onWrite, shared_from_this()));
}

//----------------------------------------------------------------------------------------------------------------------
void Session::onWrite(boost::beast::error_code errorCode, std::size_t bytesTransferred)
{
    boost::ignore_unused(bytesTransferred);

    if (errorCode)
    {
        // TODO - Fairly certain we cannot throw from the strand
        //        Would probably just log
        //        Might flag the session for closure if repeated errors. Test.
        std::cerr << "Session::onWrite called with error code: " << errorCode << std::endl;
    }

    // Clear the buffer
    m_buffer.consume(m_buffer.size());

    // Do another read
    doRead();
}
