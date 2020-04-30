#include "Listener.h"

#include <boost/asio.hpp>

#include <iostream>
#include <memory>

int main(int argc, char * argv[])
{
    // Check command line arguments
    if (argc != 4)
    {
        std::cerr <<
            "Usage: boost_beast_websocket_server_poc <address> <port> <threads>\n" <<
            "Example:\n" <<
            "    websocket-server-async 0.0.0.0 8080 1\n";
        return EXIT_FAILURE;
    }

    boost::asio::ip::address address;
    try
    {
        address = boost::asio::ip::make_address(argv[1]);
    }
    catch(boost::system::error_code &)
    {
        std::cerr << "IPv4 address argument is invalid." << std::endl;
        return EXIT_FAILURE;
    }

    unsigned short port;
    try
    {
        port = std::stoi(argv[2]);
        if (port < 1 || port > 65535)
            throw std::out_of_range("Port argument is out of range");
    }
    catch(std::exception &)
    {
        std::cerr << "Port argument is out of range or invalid." << std::endl;
        return EXIT_FAILURE;
    }

    unsigned short num_threads;
    try
    {
        num_threads = std::stoi(argv[3]);
        if (num_threads < 1 || num_threads > 65535)
            throw std::out_of_range("Number of threads argument is out of range");
    }
    catch (std::exception &)
    {
        std::cerr << "Number of threads argument is out of range or invalid" << std::endl;
    }

    // Create the IO Context
    boost::asio::io_context io_context{num_threads};

    // Launch Listener
    std::make_shared<Listener>(io_context, boost::asio::ip::tcp::endpoint{address, port})->run();

	std::cout << "Server Starting..." << std::endl;
	return 0;
}
