#pragma once

#include <boost/beast.hpp>

#include <memory>


//----------------------------------------------------------------------------------------------------------------------
class Listener : public std::enable_shared_from_this<Listener>
{
public:
    Listener(boost::asio::io_context& ioContext,
             boost::asio::ip::tcp::endpoint endpoint);

    void run();

private:
    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;

    void doAccept();
    void onAccept(boost::beast::error_code errorCode, boost::asio::ip::tcp::socket socket);
};
