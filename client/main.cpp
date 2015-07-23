#include <cotsb/client.h>
#include <cotsb/client_engine.h>
#include <cotsb/logging.h>

#include <iostream>
#include <unistd.h>

#include <thread>
#include <mutex>

int main(int argc , char *argv[])
{
    // Create the main window
    cotsb::LoggerManager::init();

    cotsb::logger % "Info" << "Starting client" << cotsb::endl;
    sf::RenderWindow window(sf::VideoMode(800, 600), "cotsb", sf::Style::Default);

    if (!cotsb::ClientEngine::init(&window))
    {
        cotsb::logger % "Error" << "Failed to init client engine!" << cotsb::endl;
        return 0;
    }
    cotsb::ClientEngine::game_loop();

    cotsb::logger % "Info" << "Shutting game down" << cotsb::endl;
    cotsb::ClientEngine::deinit();

    return EXIT_SUCCESS;

    /*
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

    auto has_message = false;
    std::string message;
    std::mutex lock;

    std::thread input_thread([] (std::mutex &lock, std::string &message, bool &has_message)
    {
        while (true)
        {
            std::string temp;
            std::cout << "Enter message: ";
            std::cin >> temp;

            std::lock_guard<std::mutex> guard(lock);
            has_message = true;
            message = temp;
        }

    }, std::ref(lock), std::ref(message), std::ref(has_message));

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

        {
            if (has_message)
            {
                sf::Packet to_send;
                {
                    std::lock_guard<std::mutex> guard(lock);
                    to_send << message;
                    has_message = false;
                }
                client.socket().send(to_send);
            }
        }
        usleep(10000);
    }
    return 0;
    */
}
