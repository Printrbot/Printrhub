//
// Created by Phillip Schuster on 27.09.16.
//

#ifndef MK20_ERRORS_H
#define MK20_ERRORS_H

enum class DownloadError {
    UnknownError = 0,
    ConnectionFailed = 1,
    Timeout = 2,
    Forbidden = 3,
    FileNotFound = 4,
    InternalServerError = 5
};

#endif //MK20_ERRORS_H
