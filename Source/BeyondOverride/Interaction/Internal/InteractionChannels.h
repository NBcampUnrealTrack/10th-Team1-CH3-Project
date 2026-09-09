#pragma once

#include "Engine/EngineTypes.h"

// 상호작용 전용 트레이스 채널
//
// 별도 채널로 두는 이유는 Visibility 채널로 쏘면 벽 바닥 파편까지 다 걸린다.
// 상호작용 물건만 반응하는 채널을 따로 두면 광선 한 번으로 끝난다.
//
//
// 채널 번호가 여기 한 곳에만 있는 이유는 나중에 다른 팀원이 GameTraceChannel1 을 먼저 쓰고 있으면
// 이 줄의 숫자만 바꾸면 된다. 다른건 고칠게 없다.
//
// 실제 채널 정의는 Config/DefaultEngine.ini 에 있다.
//   [/Script/Engine.CollisionProfile]
//   +DefaultChannelResponses=(Channel=ECC_GameTraceChannel1, ... Name="Interaction")
//
// 프로젝트 세팅 > Collision > Trace Channels 에서도 확인할 수 있다.
#define ECC_Interaction ECollisionChannel::ECC_GameTraceChannel1
