#ifndef ENGINEIO_H
#define ENGINEIO_H

#include "../common.h"


class engineIn
{
    public:

        struct control
        {
            string control_name;
            int value;
            int trigger_action;
        };

        engineIn(GLFWwindow* window);
        virtual ~engineIn();

        void UpdateIn(float delta); /*should be called every time the input system
                                      has to read all the inputs(usually every frame)*/

        void add_action(string action_name, void (*call_back)());
        void add_control(string control_name, void (*call_back)(float,float));

    protected:

    private:

        GLFWwindow* InWindow;

        map<string, void (*)()> action_map;                 //maps a name to a function
        map<int, string> key_action_map;                    //maps a key to an action name
        map<string, void (*)(float, float)> control_map;    //maps a name to a function
        map<int, control> key_control_map;                  //maps a key to a control name

        void internal_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif // ENGINEIO_H
