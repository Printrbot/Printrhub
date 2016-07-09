//
// Created by Phillip Schuster on 25.06.16.
//

#include "Idle.h"

Idle::Idle()
{

}

Idle::~Idle()
{

}

void Idle::loop()
{
    if (Serial.available())
    {
        String command = "";
        while (Serial.available()) {
            delay(3);
            char c = Serial.read();
            if (c != '\n')
            {
                command += c;
            }
            else
            {
                command.trim();
                if (command.equalsIgnoreCase("datetime"))
                {
                    Serial.println("2016-06-28 09:29:00 CEST");
                }
                else
                {
                    Serial.print("Unknown command:");
                    Serial.println(command);
                    Serial.println("-------");
                }
            }
        }
    }

    delay(10);
}

void Idle::onWillStart()
{

}

void Idle::onWillEnd()
{

}

String Idle::getName()
{
    return "Idle";
}