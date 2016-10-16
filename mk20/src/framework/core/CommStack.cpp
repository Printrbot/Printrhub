//
// Created by Phillip Schuster on 09.07.16.
//

#include "CommStack.h"
#include "Application.h"

CommStack::CommStack(Stream* port, CommStackDelegate* delegate):
_port(port),
_delegate(delegate),
_expectedPacketType(Header),
_receiveBufferIndex(0),
_packetMarker(COMM_STACK_PACKET_MARKER)
{
    pinMode(COMMSTACK_DATALOSS_MARKER_PIN,OUTPUT);
    digitalWrite(COMMSTACK_DATALOSS_MARKER_PIN,HIGH);

    pinMode(COMMSTACK_DATAFLOW_PIN,OUTPUT);
    digitalWrite(COMMSTACK_DATAFLOW_PIN,HIGH);
}

CommStack::~CommStack()
{ }

bool CommStack::readHeader(CommHeader* commHeader)
{
    int numBytes = _port->readBytes((uint8_t*)commHeader,sizeof(CommHeader));
    if (numBytes < sizeof(CommHeader))
    {
        return false;
    }

    return true;
}

bool CommStack::prepareResponse(CommHeader *commHeader, bool success)
{
    //Flip Request to Response, if this is a Response switch to next task. If all tasks
    //have been processed just return true and do nothing
    if (commHeader->commType == Request)
    {
        if (success)
        {
            commHeader->commType = ResponseSuccess;
        }
        else
        {
            commHeader->commType = ResponseFailed;
        }

        return true;
    }

    return false;
}

bool CommStack::writeHeader(CommHeader* commHeader)
{
    int numBytes = _port->write((uint8_t*)commHeader,sizeof(CommHeader));
    if (numBytes < sizeof(CommHeader))
    {
        return false;
    }

    return true;
}
/*
 * Taken from PacketSerial COBS encoding (https://github.com/bakercp/PacketSerial/blob/master/src/Encoding/COBS.h)
 */
size_t CommStack::encode(const uint8_t* source, size_t size, uint8_t* destination)
{
    size_t read_index  = 0;
    size_t write_index = 1;
    size_t code_index  = 0;
    uint8_t code       = 1;

    while(read_index < size)
    {
        if(source[read_index] == 0)
        {
            destination[code_index] = code;
            code = 1;
            code_index = write_index++;
            read_index++;
        }
        else
        {
            destination[write_index++] = source[read_index++];
            code++;

            if(code == 0xFF)
            {
                destination[code_index] = code;
                code = 1;
                code_index = write_index++;
            }
        }
    }

    destination[code_index] = code;

    return write_index;
}

/*
 * Taken from PacketSerial COBS encoding (https://github.com/bakercp/PacketSerial/blob/master/src/Encoding/COBS.h)
 */
size_t CommStack::decode(const uint8_t* source, size_t size, uint8_t* destination)
{
    size_t read_index  = 0;
    size_t write_index = 0;
    uint8_t code;
    uint8_t i;

    while(read_index < size)
    {
        code = source[read_index];

        if(read_index + code > size && code != 1)
        {
            return 0;
        }

        read_index++;

        for(i = 1; i < code; i++)
        {
            destination[write_index++] = source[read_index++];
        }

        if(code != 0xFF && read_index != size)
        {
            destination[write_index++] = '\0';
        }
    }

    return write_index;
}

/*
 * Taken from PacketSerial COBS encoding (https://github.com/bakercp/PacketSerial/blob/master/src/Encoding/COBS.h)
 */
size_t CommStack::getEncodedBufferSize(size_t sourceSize)
{
    return sourceSize + sourceSize / 254 + 1;
}

void CommStack::send(const uint8_t* buffer, size_t size, bool sendMarker)
{
    if(_port == 0 || buffer == 0 || size == 0) return;

    uint8_t _encodeBuffer[getEncodedBufferSize(size)];
    size_t numEncoded = encode(buffer, size, _encodeBuffer);

    _port->write(_encodeBuffer, numEncoded);

    if (sendMarker) {
      _port->write(_packetMarker);
    }
}

void CommStack::runTask(const uint8_t* buffer, size_t size)
{
    LOG("Running task and preparing response buffer");
    //Clear response data buffer
    memset(_sendBuffer,0,COMM_STACK_BUFFER_SIZE);

    //Trigger the application to run the task and send responded data
    uint16_t responseDataSize = 0;
    uint8_t* responseBuffer = &_sendBuffer[sizeof(CommHeader)];
    bool sendResponse = true;
    bool success = true;
    _delegate->runTask(_currentHeader,buffer,size,responseBuffer,&responseDataSize,&sendResponse,&success);

    LOG_VALUE("Running task complete, Response data size",responseDataSize);
    //Prepare header for the response
    if (sendResponse && prepareResponse(&_currentHeader, success))
    {
        if (_currentHeader.commType == Request)
        {
            LOG_VALUE("Sending Request",_currentHeader.getCurrentTask());
        }
        else if (_currentHeader.commType == ResponseSuccess)
        {
            LOG_VALUE("Sending Response Success",_currentHeader.getCurrentTask());
        }
        else if (_currentHeader.commType == ResponseFailed)
        {
            LOG_VALUE("Sending Response Failed",_currentHeader.getCurrentTask());
        }

        //Set size of responded data
        _currentHeader.contentLength = responseDataSize;
        uint16_t checkSum = 0;
        if (responseDataSize > 0)
        {
            checkSum = getCheckSum(responseBuffer,responseDataSize);
        }
        _currentHeader.setDataCheckSum(checkSum);

        //Copy the header
        memcpy(_sendBuffer,&_currentHeader,sizeof(CommHeader));

        //Send data
        send(_sendBuffer,responseDataSize+sizeof(CommHeader),true);
    }
    else
    {
        LOG("Task sequence complete");
    }
}

void CommStack::onDataPacketFailed()
{
    //Prepare header
    _currentHeader.contentLength = 0;
    _currentHeader.commType = ResponseFailed;
    _currentHeader.setDataCheckSum(0);

    //Send ResponseFailed packet
    send((uint8_t*)&_currentHeader, sizeof(CommHeader),true);
}

void CommStack::packetReceived(const uint8_t* buffer, size_t size)
{
    //Copy header into struct and test checksum
    memcpy(&_currentHeader,buffer,sizeof(CommHeader));

    //Now check if calculated checksum is equal that was sent
    if (_currentHeader.isOK()) {
        if (_currentHeader.contentLength > 0) {
          //We have data attached
          uint8_t* data = (uint8_t*)&buffer[sizeof(CommHeader)];
          uint16_t checkSum = getCheckSum(data,_currentHeader.contentLength);
          if (checkSum == _currentHeader.dataCheckSum) {

            runTask(data, _currentHeader.contentLength);
          } else {
            COMMSTACK_ERROR("Data checksums do not match, received malformed packet");
            _delegate->onCommStackError();
            onDataPacketFailed();
          }
        } else {
          //We have no data attached
          runTask(NULL,0);
        }
    } else {
      COMMSTACK_ERROR("Header checksums not ok, received malformed packet");
      _delegate->onCommStackError();
    }
}

void CommStack::process()
{
  if (_port->available() > 0)
  {
    digitalWrite(COMMSTACK_DATAFLOW_PIN,LOW);
    size_t numBytesRead = _port->readBytesUntil(0,_receiveBuffer,COMM_STACK_BUFFER_SIZE);
    COMMSTACK_SPAM("Read %d bytes",numBytesRead);

/*    for (int i=0;i<numBytesRead;i++) {
      DebugSerial.print(i);
      DebugSerial.print(":");
      DebugSerial.print((char)_receiveBuffer[i]);
      DebugSerial.print("(");
      DebugSerial.print(_receiveBuffer[i]);
      DebugSerial.println(")");
    }*/

    uint8_t _decodeBuffer[numBytesRead];
    size_t numDecoded = decode(_receiveBuffer, numBytesRead, _decodeBuffer);

    if (numDecoded == 0) {
      COMMSTACK_ERROR("Decoding of data failed");
      _delegate->onCommStackError();
      onDataPacketFailed();
    } else{
      COMMSTACK_SPAM("Received packet with size: %d, decoded size: %d",_receiveBufferIndex,numDecoded);

      //Packet received
      packetReceived(_decodeBuffer,numDecoded);
    }
    digitalWrite(COMMSTACK_DATAFLOW_PIN,HIGH);
  }
}

uint16_t CommStack::getCheckSum(const uint8_t *data, size_t size)
{
    uint16_t checkSum = 0;
    for (int i=0;i<size;i++)
    {
        checkSum += data[i];
    }
    return checkSum;
}

bool CommStack::sendMessage(CommHeader &header, size_t contentLength, const uint8_t *data)
{
    if(_port == 0) return false;

    //Calculate the data checksum and set the checksums of header
    uint16_t checkSum = getCheckSum(data,contentLength);
    header.setDataCheckSum(checkSum);

    //Calculate size of packet (header + data)
    size_t size = contentLength + sizeof(CommHeader);

    //Prepare packet in memory
    memcpy(_sendBuffer,&header,sizeof(CommHeader));
    if (contentLength > 0)
    {
      memcpy(&_sendBuffer[sizeof(CommHeader)],data,contentLength);
    }

    //Send data
    send(_sendBuffer,size,true);

    return true;
}

bool CommStack::requestTask(TaskID task, size_t contentLength, const uint8_t *data)
{
    LOG_VALUE("Request Task without data with ID",task);
    CommHeader header(task,contentLength);

    return sendMessage(header,contentLength,data);
}

bool CommStack::requestTask(TaskID task)
{
    LOG_VALUE("Request Task without data with ID",task);
    CommHeader header(task, 0);

    return sendMessage(header);
}

bool CommStack::responseTask(TaskID task, size_t contentLength, const uint8_t *data, bool success)
{
    LOG_VALUE("Response Task with data with ID",task);
    CommHeader header(task,contentLength);

    //Set as response
    if (success)
    {
        header.commType = ResponseSuccess;
    }
    else
    {
        header.commType = ResponseFailed;
    }

    return sendMessage(header,contentLength,data);
}

bool CommStack::responseTask(TaskID task, bool success)
{
    LOG_VALUE("Response Task without data with ID",task);
    CommHeader header(task, 0);

    //Set as response
    if (success)
    {
        header.commType = ResponseSuccess;
    }
    else
    {
        header.commType = ResponseFailed;
    }


    return sendMessage(header);
}

void CommStack::beginBlockPort() {
  digitalWrite(COMMSTACK_DATAFLOW_PIN, LOW);
}

void CommStack::endBlockPort() {
  digitalWrite(COMMSTACK_DATAFLOW_PIN, HIGH);
}