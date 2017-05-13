#pragma once
#include <stdexcept>
#include <ItemTools.h>

#ifdef _MSC_VER
#define GSB_NOVTABLE __declspec(novtable)
#define GSB_ABSTRACT abstract
#elif
#define GSB_NOVTABLE /*__declspec(novtable)*/
#define GSB_ABSTRACT
#endif

class NotImplemented : public std::logic_error
{
public:
  NotImplemented() : logic_error("Function not yet implemented.") { }
};

//template<typename ... Args>
//class IHolderHelper : public Args
//{
//public:
//
//};
//
//using EmptyHolder = IHolderHelper<>;
//using GuiDefaultHolders = IHolderHelper<class DBHolder>;