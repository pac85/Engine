#include "logger.h"

logger::logger(string log_file_path)
{
    remove(log_file_path.c_str());      //removes old log file
    log_file.open(log_file_path, ios::out);
}

logger::~logger()
{
    log_file.close();
}

logger::log_opt::log_opt()
{

}

logger::log_opt::log_opt(bool _err_quit, bool _console, bool _file)
{
    console = _console, file = _file, err_quit = _err_quit;
}

logger::log_data::log_data(log_opt _opt, string _msg, log_type _type)
{
    opt = _opt, msg = _msg, type = _type;
}

void logger::log_message(string msg, log_opt opt)
{
    if(opt.console)
    {
        cout << msg << endl;
    }
    if(opt.file)
    {
        log_file << "       : " <<msg << endl;
    }

    log.push_back(log_data(opt, msg, normal));      //log messages are always added to the log so they can be displayed
                                                    //in the in game console and used by other parts of the engine
    if(opt.err_quit)
    {
        exit(-1);
        log_file.flush();
    }
}

void logger::log_warning(string msg, log_opt opt)
{
    if(opt.console)
    {
        cout << msg << endl;
    }
    if(opt.file)
    {
        log_file << "warning: " << msg << endl;
    }

    log.push_back(log_data(opt, msg, warning));      //log messages are always added to the log so they can be displayed
                                                     //in the in game console and used by other parts of the engine
    if(opt.err_quit)
    {
        exit(-1);
        log_file.flush();
    }
}

void   logger::log_error(string msg, log_opt opt)
{
    if(opt.console)
    {
        cerr << msg << endl;
    }
    if(opt.file)
    {
        log_file << "error  : " << msg << endl;
    }

    log.push_back(log_data(opt, msg, error));      //log messages are always added to the log so they can be displayed
                                                    //in the in game console and used by other parts of the engine
    if(opt.err_quit)
    {
        exit(-1);
        log_file.flush();
    }
}

void logger::operator << (msg in)
{
    log_message(in.data, log_opt(false, true));
}

void logger::operator << (warn in)
{
    log_warning(in.data, log_opt(false, true));
}

void logger::operator << (err in)
{
    log_error(in.data, log_opt(true, true));
}

/*string logger::operator << (string in)
{

}*/


