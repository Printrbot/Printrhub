//
// Created by Phillip Schuster on 09.07.16.
//

#ifndef ESP8266_ARM_SWD_COMMSTACK_H
#define ESP8266_ARM_SWD_COMMSTACK_H

#include "Arduino.h"

//Maximum is 255 as currentTaskIndex is a byte
#define COMM_STACK_MAX_TASKS 10
#define COMM_STACK_PACKET_MARKER 0x00
#define COMM_STACK_BUFFER_SIZE 256

enum class Compression: uint8_t {
    None = 1,
    RLE16 = 2
};

enum CommType : uint8_t {
    Request = 1,
    ResponseSuccess = 2,
    ResponseFailed = 3
};

enum PacketType : uint8_t {
    Header = 0,
    Data = 1
};

enum class TaskID : uint8_t {
    Unknown = 0, // is this one needed?
    GetIndexFile = 1,
    GetProjectWithID = 2,
    PrintProjectWithID = 3,
    GetTimeAndDate = 4,
    GetProjectItemWithID = 5,
    GetJobWithID = 6,
    FileOpenForWrite = 7,
    FileSaveData = 8,
    FileClose = 9,
    PrinterCommand = 10,
    FlushBuffer = 11,
    StartWifi = 12,
    StopWifi = 13,
    UpdateMK20Firmware = 14,
    UpdateESPFirmware = 15,
    RunPrinterGCode = 16,
    Error = 17,
    SaveProjectWithID = 18,
    FileSetSize = 19,
    DownloadFile = 22,
    DownloadError = 23,
    StartFirmwareUpdate = 24,
    Ping = 25,
    ShowFirmwareUpdateNotification = 26,
    FirmwareUpdateError = 27,
    DebugLog = 28,
    RestartESP = 29,
    FirmwareUpdateComplete = 30,
    DownloadFileToSDCard = 31,
    ShowFirmwareUpdateInProgress = 32,
    GetSystemInfo = 33
};

struct CommHeader {
public:
    uint8_t taskID;
    uint8_t commType;
    uint8_t contentLength;
    uint16_t dataCheckSum;
    uint16_t checkSum;

public:
    CommHeader() {
        this->commType = Request;
        this->contentLength = 0;
        this->dataCheckSum = 0;
        updateCheckSum();
    }

    CommHeader(TaskID task, uint8_t contentLength) {
        this->taskID = (uint8_t)task;
        this->commType = Request;
        this->contentLength = contentLength;
        this->dataCheckSum = 0;
        updateCheckSum();
    }

    CommHeader(TaskID* tasks, uint8_t numberOfTasks, uint8_t contentLength) {
        this->taskID = (uint8_t)tasks[0];
        this->commType = Request;
        this->contentLength = contentLength;
        this->dataCheckSum = 0;
        updateCheckSum();
    }

    TaskID getCurrentTask()
    {
        return (TaskID)taskID;
    }

    bool isFinished()
    {
        return (commType == ResponseSuccess || commType == ResponseFailed);
    }

    void setDataCheckSum(uint16_t checkSum)
    {
        this->dataCheckSum = checkSum;
        updateCheckSum();
    }

    bool isOK() {
        return (checkSum == calculateCheckSum());
    }

private:
    uint16_t calculateCheckSum() {
        return this->taskID + this->commType + this->contentLength + this->dataCheckSum;
    }

    void updateCheckSum() {
        this->checkSum = calculateCheckSum();
    }
};

class CommStackDelegate
{
public:
    virtual bool runTask(CommHeader& header, const uint8_t* data, size_t dataSize, uint8_t* responseData, uint16_t* responseDataSize, bool* sendResponse, bool* success) = 0;
    virtual void onCommStackError() = 0;
};

class CommStack
{
/*#pragma mark Task Definitions
public:
    static const uint8_t GetIndexFile = 0;
    static const uint8_t GetProjectWithID = 1;
    static const uint8_t PrintProjectWithID = 2;
    static const uint8_t GetTimeAndDate = 3;
*/
#pragma mark Constructor
public:
    CommStack(Stream* port, CommStackDelegate* delegate);
    ~CommStack();

public:
    void process();
    bool requestTask(TaskID task, size_t contentLength, const uint8_t* data);
    bool requestTask(TaskID task);
    bool responseTask(TaskID task, bool success);
    bool responseTask(TaskID task, size_t contentLength, const uint8_t* data, bool success);
    bool sendMessage(CommHeader& header, size_t contentLength=0, const uint8_t* data=NULL);
    bool requestTasks(TaskID* tasks);
    Stream* getPort() const { return _port; };

    void beginBlockPort();
    void endBlockPort();

private:
    bool readHeader(CommHeader* commHeader);
    bool prepareResponse(CommHeader* commHeader, bool success);
    bool writeHeader(CommHeader* commHeader);
    void packetReceived(const uint8_t* buffer, size_t size);
    size_t getEncodedBufferSize(size_t sourceSize);
    size_t encode(const uint8_t* source, size_t size, uint8_t* destination);
    size_t decode(const uint8_t* source, size_t size, uint8_t* destination);
    void runTask(const uint8_t* buffer, size_t size);
    void onDataPacketFailed();
    void send(const uint8_t* buffer, size_t size, bool sendMarker);
    uint16_t getCheckSum(const uint8_t* data, size_t size);

#pragma mark Member Variables
private:
    Stream* _port;
    CommStackDelegate* _delegate;
    uint8_t _receiveBuffer[COMM_STACK_BUFFER_SIZE];
    uint8_t _sendBuffer[COMM_STACK_BUFFER_SIZE];

    size_t _receiveBufferIndex;
    CommHeader _currentHeader;
    PacketType _expectedPacketType;
    uint8_t _packetMarker;
};

#endif //ESP8266_ARM_SWD_COMMSTACK_H
