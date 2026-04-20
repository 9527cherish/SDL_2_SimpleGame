# SimpleGame

`images` 路径下的 `tmwa` 贴图资源来自 The Mana World (TMW)：

`https://github.com/themanaworld/tmwa-client-data`

## 当前功能

- SDL2 客户端角色预览与场景渲染
- 基于 Muduo 的简单联机同步
- TMWA 角色 XML 解析与分层人物渲染
- 游戏场景背景图与 camera 跟随

## 联机说明

- 先启动服务端：`./bin/server`
- 再启动一个或多个客户端：`./bin/rougeClient`
- 在主菜单选择角色后点击“开始游戏”，当前客户端会注册到服务端
- 其他客户端进入游戏后，会同步看到已经在线的角色
- 远端客户端移动或切换动作时，本地会同步更新对应角色

## 操作说明

- `W A S D` / 方向键 / 小键盘 `8 4 2 6`：持续移动
- `J`：攻击最近的树木
- `K`：攻击最近的怪物
- `G`：拾取最近的掉落物
- `Space`：`attack`
- `4`：`attack_sword_stab`
- `5`：`attack_bow`
- `6`：`attack_spear`
- `7`：`attack_chop`
- `8`：`attack_2hand`
- `9` 或 `C`：`cast`
- `1`：切换到 `stand`
- `2`：切换到 `sit`
- `3`：切换到 `dead`

## 本次行为调整

- 角色移动由原来的“按键事件触发一次位移”改为“按键状态驱动的逐帧连续移动”，行走会更平滑
- 本机角色固定显示在屏幕中心，背景与远端角色按 camera 偏移渲染
- 已补齐并接入 TMWA XML 中常见的人物动作：
  - `stand`
  - `walk`
  - `sit`
  - `dead`
  - `attack`
  - `attack_sword_stab`
  - `attack_bow`
  - `attack_spear`
  - `attack_chop`
  - `attack_2hand`
  - `cast`
- 一次性动作会在短暂播放后自动回到 `stand` 或 `walk`，避免动作停在中间帧
- 服务端现在会同步怪物状态与掉落物状态，客户端会渲染怪物血条与地面掉落物

## 已知说明

- 不同部件 XML 的动作覆盖范围不完全一致，个别装备部件如果没有某个动作，会按当前代码里的回退逻辑退回到可用动作帧
- 当前背景使用 TMWA 的 `graphics/images/login_wallpaper.png`，属于单张大图裁切，不是完整瓦片地图

## 后续可扩展方向

- 增加完整地图与碰撞
- 增加公共聊天与私聊
- 增加怪物、NPC 与技能表现同步

## 当前架构

- `include/common/protocol/`
  统一放消息号、玩家同步协议、树木同步协议、通用封包解析，client/server 共用。
- `client`
  资源、会话、远端玩家、树木状态分别拆到了 `DataManager/` 下的小模块中。
  `NetClient` 现在只负责联机会话，底层收发由 `ClientTransport` 负责。
  `GameScene` 只保留场景壳，世界渲染与 camera 跟随收敛到 `WorldScenePresenter`。
- `server`
  `GameServer` 负责 Muduo 入口与网络回调。
  `GameService` 负责编排，但连接管理、消息路由、玩家存储、树木领域状态已经拆分为独立模块：
  `ServerConnectionRegistry`、`ServerMessageRouter`、`ServerPlayerStore`、`ServerTreeManager`。

## 重构说明

- 旧的 `DataManager` 兼容门面已经移除，客户端直接依赖 `AssetRepository`、`PlayerSession`、`RemotePlayerStore`。
- 旧的 `characterManager.cpp` / `characterManager.hpp` 已移除，玩家状态由 `ServerPlayerStore` 接管。
- 当前工程已经完成“协议层拆分、客户端状态层拆分、客户端传输层拆分、世界渲染层拆分、服务端连接与领域层拆分”这几轮重构，可以继续在现有目录上扩展怪物、地图物件和更多同步消息。

## 目录约束

- 新的联机协议统一放在 `include/common/protocol/`，不要再回写到旧的聚合结构体文件中。
- 客户端场景层只做界面切换和输入分发，世界更新与渲染优先下沉到独立 presenter / manager。
- 服务端入口层只负责收包和转发到服务层，具体玩家、树木、后续怪物逻辑都按领域拆到独立 store / service。

## 模块指南

- `AssetRepository`
  负责异步加载人物模板资源，只存“可复用模板”，不要在这里放当前玩家状态。
- `PlayerSession`
  只负责本机当前角色选择、当前 `Persona` 与会话内状态。
- `RemotePlayerStore`
  只负责远端玩家快照、远端 `RemotePersona` 生命周期与同步落地。
- `TreeManager`
  只负责客户端树木快照与渲染缓存，不负责服务器判定。
- `ClientTransport`
  只负责 socket 收发与拆包，不直接改游戏状态。
- `NetClient`
  作为联机会话门面，把传输层消息转成客户端状态变更。
- `WorldScenePresenter`
  负责游戏世界渲染、camera 跟随、本机角色与远端角色的场景组织。
- `GameServer`
  只负责 Muduo 接入点、连接回调和消息入口。
- `GameService`
  只负责服务端编排，不直接承担底层连接存储和所有领域状态。
- `ServerConnectionRegistry` / `ServerPlayerStore` / `ServerTreeManager`
  分别负责连接映射、玩家仓库、树木领域状态，新增怪物或掉落物时按同样模式扩展。

## 单例约束

- 只有“天然全局唯一”的模块才保留单例：资源仓库、当前会话、远端仓库、UI 管理器、联机会话门面。
- 单例类不承载彼此重叠的职责，例如 `NetClient` 不直接持有远端人物容器，`PlayerSession` 不负责资源加载。
- 新增功能如果只是一个独立领域对象，优先做普通成员或普通 service，不要默认继续做成单例。
