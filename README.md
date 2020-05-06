# cplusplus_websockets
Proof of concept code that allows a C++ application to communicate over websockets

This project is currently being built using boost::beast.
Therefore, you must have boost installed with the header and lib directories set up properly in the compiler you use to build this project.

Windows:  
I personally use Visual Studio 2019 and have used vcpkg to install boost.  
Install instructions can be found here:  
https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019  

After vcpkg is installed, along with Visual Studio, simply type on the command line:

vcpkg install boost:x64-windows  
vcpkg integrate install  

After that, you can open the solution file and build as normal.

Linux:  
I used a hyper-v installation of Ubuntu 18.04  
I installed all the gcc tools, cmake, etc.  
You will have to download and build boost yourself.  
Edit the CmakeLists.txt BOOST_ROOT variable to point to the boost directory  

To run the program, open a terminal and use this command:  
boost_beast_websocket_server_poc <address> <port> <threads>  

For example:  
boost_beast_websocket_server_poc 127.0.0.1 8080 2  
