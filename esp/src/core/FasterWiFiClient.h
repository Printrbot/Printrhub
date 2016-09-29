//
// Created by Phillip Schuster on 29.09.16.
//

#ifndef ESP_FASTERWIFICLIENT_H
#define ESP_FASTERWIFICLIENT_H

#include <memory>
#include "Arduino.h"
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"
#include "include/slist.h"

#define WIFICLIENT_MAX_PACKET_SIZE 1460

class ClientContext;
class WiFiServer;

class FasterWiFiClient : public Client, public SList<FasterWiFiClient> {
protected:
    FasterWiFiClient(ClientContext* client);

public:
    FasterWiFiClient();
    virtual ~FasterWiFiClient();
    FasterWiFiClient(const FasterWiFiClient&);
    FasterWiFiClient& operator=(const FasterWiFiClient&);

    uint8_t status();
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char *host, uint16_t port);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    size_t write_P(PGM_P buf, size_t size);
    template <typename T>
    size_t write(T& source, size_t unitSize);

    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buf, size_t size);
    virtual int peek();
    virtual size_t peekBytes(uint8_t *buffer, size_t length);
    size_t peekBytes(char *buffer, size_t length) {
        return peekBytes((uint8_t *) buffer, length);
    }
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();

    IPAddress remoteIP();
    uint16_t  remotePort();
    IPAddress localIP();
    uint16_t  localPort();
    bool getNoDelay();
    void setNoDelay(bool nodelay);
    static void setLocalPortStart(uint16_t port) { _localPort = port; }

    template<typename T> size_t write(T &src){
        uint8_t obuf[WIFICLIENT_MAX_PACKET_SIZE];
        size_t doneLen = 0;
        size_t sentLen;
        int i;

        while (src.available() > WIFICLIENT_MAX_PACKET_SIZE){
            src.read(obuf, WIFICLIENT_MAX_PACKET_SIZE);
            sentLen = write(obuf, WIFICLIENT_MAX_PACKET_SIZE);
            doneLen = doneLen + sentLen;
            if(sentLen != WIFICLIENT_MAX_PACKET_SIZE){
                return doneLen;
            }
        }

        uint16_t leftLen = src.available();
        src.read(obuf, leftLen);
        sentLen = write(obuf, leftLen);
        doneLen = doneLen + sentLen;
        return doneLen;
    }

    friend class WiFiServer;

    using Print::write;

    static void stopAll();
    static void stopAllExcept(FasterWiFiClient * c);

protected:

    static int8_t _s_connected(void* arg, void* tpcb, int8_t err);
    static void _s_err(void* arg, int8_t err);

    int8_t _connected(void* tpcb, int8_t err);
    void _err(int8_t err);

    ClientContext* _client;
    static uint16_t _localPort;
};


template <typename T>
inline size_t FasterWiFiClient::write(T& source, size_t unitSize) {
    std::unique_ptr<uint8_t[]> buffer(new uint8_t[unitSize]);
    size_t size_sent = 0;
    while(true) {
        size_t left = source.available();
        if (!left)
            break;
        size_t will_send = (left < unitSize) ? left : unitSize;
        source.read(buffer.get(), will_send);
        size_t cb = write(buffer.get(), will_send);
        size_sent += cb;
        if (cb != will_send) {
            break;
        }
    }
    return size_sent;
}

#endif //ESP_FASTERWIFICLIENT_H
