#pragma once

#include <SFML/Network.hpp>
#include <memory>
#include <functional>

#include <cotsb/commands.h>

namespace cotsb
{
    class Packet 
    {
        public:

            typedef std::unique_ptr<sf::Packet> UniquePacket;

            typedef std::function<void (Packet &packet)> PacketSendCallback;

            enum Type
            {
                Single,
                Broadcast,
                Callback
            };

            inline Packet(Commands::Type command, sf::TcpSocket *socket, Type type) :
                _command(command),
                _type(type),
                _socket(socket),
                _data(UniquePacket(new sf::Packet()))
            {
                *_data << static_cast<CommandType>(command);
            }
            inline Packet(Commands::Type command, PacketSendCallback callback) :
                _command(command),
                _type(Callback),
                _socket(nullptr),
                _callback(callback),
                _data(UniquePacket(new sf::Packet()))
            {
                *_data << static_cast<CommandType>(command);
            }

            inline Commands::Type command() const
            {
                return _command;
            }
            inline Type type() const
            {
                return _type;
            }
            inline sf::TcpSocket *socket() const
            {
                return _socket;
            }
            inline PacketSendCallback callback() const
            {
                return _callback;
            }
            inline sf::Packet *data() const
            {
                return _data.get();
            }

        private:

            Commands::Type _command;
            Type _type;

            sf::TcpSocket *_socket;
            PacketSendCallback _callback;
            UniquePacket _data;

    };

}
