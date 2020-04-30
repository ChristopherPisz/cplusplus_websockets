# cplusplus_websockets
Proof of concept code that allows a C++ application to communicate over websockets

This project is currently being built using boost::beast.
Therefore, you must have boost installed with the header and lib directories set up properly in the compiler you use to build this project.

I personally use Visual Studio 2019 and have used vcpkg to install boost.
Install instructions can be found here:
https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019

After vcpkg is installed, along with Visual Studio, simply type on the command line:

vcpkg install boost:x64-windows  
vcpkg integrate install  

After that, you can open the solution file and build as normal.
