#include <iostream>
#include <cotsb/server.h>
#include <thread>
#include <functional>
#include <sstream>

int main(int argc , char *argv[])
{
    cotsb::Stream stream;

    std::thread write_thread([] (cotsb::Stream &stream)
    {
        for (auto i = 0u; i < 100u; i++)
        {
            std::stringstream ss;
            ss << "Data: " << i;
            std::cout << "Writing: " << i << "\n";
            stream.write(ss.str());
        }
    }, std::ref(stream));
    
    std::thread read_thread([] (cotsb::Stream &stream)
    {
        for (auto i = 0u; i < 50u; i++)
        {
            std::stringstream ss;
            auto read = stream.read(ss, 2000);
            std::cout << "Read: " << ss.str() << " : " << read << "\n"; 
        }
    }, std::ref(stream));

    write_thread.join();
    read_thread.join();

    std::cout << "Done\n";
    //cotsb::Server server;
    //server.connect();
}
 
