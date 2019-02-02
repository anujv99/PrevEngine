#pragma once

#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <memory>
#include <algorithm>

#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <array>
#include <initializer_list>

#ifdef PV_PLATFORM_WINDOWS
#include <Windows.h>
#include <windowsx.h>
#include <winerror.h>
#endif

#include "engine/log.h"
#include "engine/assert.h"
#include "essentials/timer.h"

#include "engine/events/applicationevent.h"
#include "engine/events/keyevent.h"
#include "engine/events/mouseevent.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
