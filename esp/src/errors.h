/*
 * Structure for error handling
 *
 * Copyright (c) 2016 Printrbot Inc.
 *
 * Developed in cooperation by Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
    PrepareDownloadedFileFailed = 7,

    MK20UpdateError = 100,

    ESPUpdateNoUpdates = 200,
    ESPUpdateFailed = 201,

    LocalFileNotFound = 300,
    LocalFileOpenForReadFailed = 301,

    TargetFileOpenForWriteFailed = 400
};

enum class FirmwareUpdateError {
    UnknownError = 0,
};

#endif //ESP_ERRORS_H
