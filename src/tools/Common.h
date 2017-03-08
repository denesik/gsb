#pragma once
#include <stdexcept>

typedef uint32_t ItemId;

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