#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//   ==================================================
extern bool bDebug;
extern float DeltaTime;
extern bool bFrameLimit;
extern bool bDeltaTimeUnFixed;
extern bool bDebugTimeScale;

#include <memory>
#include <mutex>
#include <map>
#include <list>
#include <unordered_map>
#include <set>
#include <array>
#include <execution>

#include <algorithm>
#include <functional>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <sstream>
#include <string_view>
#include <optional>
#include <any>
#include <variant>
#include <numeric>
#include <numbers>
#include <cmath>
#include <random>





