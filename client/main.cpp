#include <cotsb/client.h>

#include <iostream>
#include <unistd.h>

int main(int argc , char *argv[])
{
    cotsb::Client client(8888);
    
    auto connect_result = sf::Socket::NotReady;
    while (connect_result == sf::Socket::NotReady)
    {
        connect_result = client.start_client();
        if (connect_result == sf::Socket::NotReady)
        {
            usleep(10000);
        }
    }

    if (connect_result != sf::Socket::Done)
    {
        std::cout << "Error connecting to server.\n";
        return 1;
    }

    std::cout << "Connected!\n";

    while (true)
    {
        client.check_network();
        if (client.new_data().getDataSize() > 0)
        {
            std::cout << "Has " << client.new_data().getDataSize() << " bytes\n";
            std::string message;
            client.new_data() >> message;
            std::cout << "Message: " << message << "\n";
        }
        usleep(10000);
    }
    return 0;
}
