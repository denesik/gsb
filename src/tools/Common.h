#pragma once
#include <stdexcept>

#ifdef _MSC_VER
#define GSB_NOVTABLE __declspec(novtable)
#elif
#define GSB_NOVTABLE /*__declspec(novtable)*/
#endif

class NotImplemented : public std::logic_error
{
public:
  NotImplemented() : logic_error("Function not yet implemented.") { }
};