// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

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
#include "Player.h"



/*
일정
서버 - 로비 - 로그인 순 추천

프레임 수 60? 30?

단계별로 진행하자

데이터 사이즈가 작아 nagle 알고리즘 때문에 안될수 있다.

원격 네트워크 전송이 안될땐 네트워크 설정 (방화벽) 체크
원격 네트워크 테스트 해볼것.
*/

