#pragma once

#include <boost/beast.hpp>

#include <memory>


//----------------------------------------------------------------------------------------------------------------------
// Represents a client connection and implements the send and receive
class Session : public std::enable_shared_from_this<Session>
{
    boost::beast::websocket::stream<boost::beast::tcp_stream> m_websocket;
    boost::beast::flat_buffer m_buffer;

public:

    // Move Constructor takes ownership of the socket
    explicit Session(boost::asio::ip::tcp::socket&& socket);

    // Get on the correct executor
    void run();

    // Start the asynchronous operation
    void onRun();

    void onAccept(boost::beast::error_code error_code);

    void doRead();

    void onRead(boost::beast::error_code errorCode, std::size_t bytesTransferred);

    void onWrite(boost::beast::error_code errorCode, std::size_t bytesTransferred);
};
