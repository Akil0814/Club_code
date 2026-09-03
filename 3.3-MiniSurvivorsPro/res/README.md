# Resource Naming

资源文件统一使用小写 `snake_case`，动画帧从 `00` 开始并使用两位补零：

```text
<action>_<frame>.png
```

角色和敌人动画默认朝左。向右显示时由代码水平翻转，不保存重复的右向资源。

目录职责：

- `audio/`：背景音乐和音效。
- `world/`：游戏场景背景。
- `players/`：玩家动画与共享阴影。
- `enemies/`：敌人动画与共享阴影。
- `props/`：场景道具动画。

当前游戏使用 `player_01` 和 Boar 移动动画。`player_02`、Boar Idle、Skullwolf Move/Death 和道具动画为后续功能预留。

