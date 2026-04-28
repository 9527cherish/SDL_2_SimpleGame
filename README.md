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

## 已知说明

- 不同部件 XML 的动作覆盖范围不完全一致，个别装备部件如果没有某个动作，会按当前代码里的回退逻辑退回到可用动作帧
- 当前背景使用 TMWA 的 `graphics/images/login_wallpaper.png`，属于单张大图裁切，不是完整瓦片地图

## 后续可扩展方向

- 增加完整地图与碰撞
- 增加公共聊天与私聊
- 增加怪物、NPC 与技能表现同步
