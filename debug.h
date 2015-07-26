// wxWidgets precompiled / standard headers
#include "wx/wxprec.h"

// When debugging changes all calls to "new" to be calls to "DEBUG_NEW" allowing for memory leaks to
// give you the file name and line number where it occurred.
#ifdef _DEBUG
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

#else
#define DEBUG_NEW new
#endif