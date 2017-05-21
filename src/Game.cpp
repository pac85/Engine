#include "Game.h"

Game::Game()
{
    //ctor
    game.GameSetup();
}

Game::~Game()
{
    //dtor
}

void Game::message::check_ack()
{
    //checks if the messages have been acknowledged the maximum amount of time
    bool ack = true;
    for(map<string, int>::iterator rec_it = receivers.begin();rec_it != receivers.end();++rec_it)
    {
        if(rec_it->second > 0)
        {
            ack = false;
            return;  //if one is different from zero no need to check the others
        }
    }
    if(ack)          //suicide(delete itself) if acknowledged
    {
        delete[](this);
    }
}

void Game::Use_camera(Camera* _camera)
{
    current_camera = _camera;
}

void Game::add_message(message* msg_too_add)
{
    map<string, int>::iterator rec_it;
    //for every element of the buffer map witch has a corresponding receiver
    //pushes the given message in the corresponding vector
    for(rec_it = msg_too_add->receivers.begin();rec_it != msg_too_add->receivers.end();++rec_it)
    {
        msg_buffer[rec_it->first].push_back(msg_too_add);
    }
}

vector<Game::message*> Game::receive_messages(string receiver_id)
{
    for(unsigned int i=0;i < msg_buffer[receiver_id].size();i++)
    {
        if(msg_buffer[receiver_id][i]->receivers[receiver_id]-- <= 0)
        {
            msg_buffer[receiver_id][i]->check_ack();    //avoids memory leak
            msg_buffer[receiver_id].erase(msg_buffer[receiver_id].begin() + i) ;
        }
    }
    return msg_buffer[receiver_id];
}
