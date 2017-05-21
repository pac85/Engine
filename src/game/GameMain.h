#ifndef GAMEMAIN_H
#define GAMEMAIN_H

#include"../common.h"

/* include all your actors headers here*/

/***************************************/


class GameMain
{
    public:
        GameMain();
        virtual ~GameMain();

        void GameSetup();   //called once a the beginning of the game

    protected:

    private:
};

#endif // GAMEMAIN_H
