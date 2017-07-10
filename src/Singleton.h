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
template<typename T>
class Singleton
{
   static T* ms_singleton;
   public:
      Singleton()
      {
     	assert(!ms_singleton);
     	//use a cunning trick to get the singleton pointing to the start of
     	//the whole, rather than the start of the Singleton part of the object
     	int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
     	ms_singleton = (T*)((int)this + offset);
      }
      ~Singleton()
      {
     	assert(ms_singleton);
     	ms_singleton=0;
      }
      static T& GetSingleton()
      {
     	assert(ms_singleton);
     	return *ms_singleton;
      }
      static T* GetSingletonPtr()
      {
     	assert(ms_singleton);
     	return ms_singleton;
      }
};

template <typename T> T* Singleton <T>::ms_singleton = 0;
