/**************************************************************************/
/*
  Copyright (C) 2017 Antonino Maniscalco (alias pac85)

  This file is part of Engine.

  Engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Engine.  If not, see http://www.gnu.org/licenses/

*/
/**************************************************************************/
#ifndef GAME_H
#define GAME_H

#include"../common.h"
#include"render/Camera.h"

#include "game/GameMain.h"

class Game
{
    public:


        struct message                                  //used for hight latency comunication between classes
        {
            map<string, int> receivers;                 /*the map's key is the type of the receivers that can receive this message(a receiver id),
                                                          the value is the number of times it can be acknowledged*/

            bool broadcast;                             //if true any receiver can receive this message

            string command, arguments;                  //the content itself of the message divided in command and aruments

            void* payload;                              //a void pointer for any type of data the message has to carry

            void check_ack();                           //destroys itself if the message has been acknowledged by all receivers
        };

        void add_message(message* msg_too_add);        //adds the given message to the buffer so it can be received
        vector<message*> receive_messages(string receiver_id);  //returns a vector containing all the messages with a corresponding receiver_id(NULL if none of them correspond)



        Game();
        virtual ~Game();

        void Use_camera(Camera*);                       //sets the camera given has argument as the current camera

    protected:

    private:

        map<string, vector<message*>> msg_buffer;

        GameMain game;
        Camera * current_camera;
        BaseActor * current_player;
};

#endif // GAME_H
