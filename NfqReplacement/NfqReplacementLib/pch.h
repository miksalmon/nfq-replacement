#pragma once

#include <unknwn.h>

// WinBase.h has GetCurrentTime defined as a macro, which causes compile error (Windows.UI.Xaml.Media.Animation.h). Undefine it for now. Need to talk to
// C++/Winrt group about it.
#undef GetCurrentTime

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>

#include "common.h"

// Please don't include our own project WinRT header files eg. winrt/Utils.h
