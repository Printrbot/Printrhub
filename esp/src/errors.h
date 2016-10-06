//
// Created by Phillip Schuster on 27.09.16.
//

#ifndef ESP_ERRORS_H
#define ESP_ERRORS_H

enum class DownloadError {
    UnknownError = 0,
    ConnectionFailed = 1,
    Timeout = 2,
    Forbidden = 3,
    FileNotFound = 4,
    InternalServerError = 5,
    RemoveOldFilesFailed = 6,
    PrepareDownloadedFileFailed = 7
};

enum class FirmwareUpdateError {
    UnknownError = 0
};

#endif //ESP_ERRORS_H
