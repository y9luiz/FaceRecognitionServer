#pragma once

#include <unordered_map>
#include <functional>
#include <vector>

class MessageHandler
{
    public:
        using MessageHandlerCallbackMap =  std::unordered_map<uint8_t, std::function<void(std::vector<uint8_t> &&)>>;
        
        MessageHandler() = default;
        ~MessageHandler() = default;

        void registerCallback(uint8_t code, std::function<void(std::vector<uint8_t> &&)> callback);
        void processMessage(std::vector<uint8_t> && message);
    private:
        void invokeCallback(uint8_t code, std::vector<uint8_t> && message);

        MessageHandlerCallbackMap m_messageHandlerCallbackMap;
};