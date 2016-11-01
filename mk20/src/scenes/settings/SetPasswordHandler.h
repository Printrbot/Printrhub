//
// Created by Phillip Schuster on 01.11.16.
//

#ifndef MK20_SETPASSWORDHANDLER_H
#define MK20_SETPASSWORDHANDLER_H

#include "VirtualKeyboardSceneController.h"

class SetPasswordHandler: public VirtualKeyboardHandler {
public:
    void onFinished(String textEntered);
    void onCancelled();
};


#endif //MK20_SETPASSWORDHANDLER_H
