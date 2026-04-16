#pragma once

// 兼容聚合头：
// 新协议优先直接包含 include/common/protocol/ 下的具体头文件。
// 这里仅保留统一入口，不再继续新增业务结构。

#include "common/protocol/msg_types.hpp"
#include "common/protocol/player_protocol.hpp"
#include "common/protocol/tree_protocol.hpp"
#include "common/protocol/packet_protocol.hpp"
