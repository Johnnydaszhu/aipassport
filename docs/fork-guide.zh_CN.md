<p align="right">
  <strong>简体中文</strong> · <a href="fork-guide.md">English</a>
</p>

# 下游开发工作流

本仓库是基于 `FoloToy/ai-passport` 的下游产品实现。`main` 保存当前 TheGreatMe 终端
固件，因此有意与上游硬件基线保持差异。

## 仓库职责

```text
docs/                  产品、贡献、开发与设计文档
components/bsp/        稳定板级 API 与硬件实现
main/                  TheGreatMe 终端应用与诊断 Demo
assets/                可复用字体、图片、音乐与音效
skills/                可复用 AI agent 技能
tests/                 可在主机运行的逻辑测试
sdkconfig.defaults     可复现的 ESP32-C3 默认配置
```

根目录 `README.md` 双语文件负责说明当前产品；继承自上游的 `docs/README.md` 双语文件
保留详细硬件能力契约。

## 分支与上游约定

- 把本仓库的 `main` 视为产品分支；普通改动从它创建短生命周期的 `feature/*`、
  `fix/*` 或 `docs/*` 分支。
- 将官方基线配置为只读的 `upstream` remote：
  `https://github.com/FoloToy/ai-passport.git`。
- 在独立分支审查上游更新后再合入产品；明确处理应用、分区、BLE 和文档差异，并运行
  完整验证门禁。
- 禁止使用无人值守任务把 `upstream/main` 自动合入本产品的 `main`。
- 可复用板级逻辑放在 `components/bsp`；TheGreatMe 应用行为放在 `main`。

`docs/assets/` 用于补充根 README 的产品架构、设计资料与图片。可复用媒体放入
`assets/` 对应子目录，并记录来源与许可证。

产品特有资料保留在本仓库。通用硬件事实、可复用接口、构建优化和能帮助所有 AI
Passport 用户的经验，可通过独立 PR 回馈上游。`plays/` 归档与发布后经验流程见
`docs/development/project-completion.md`。

所有维护中的文档均以默认 `.md` 保存英文、以 `.zh_CN.md` 保存简体中文，并保留双向
语言链接。
