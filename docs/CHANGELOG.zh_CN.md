<p align="right">
  <strong>简体中文</strong> · <a href="CHANGELOG.md">English</a>
</p>

# Changelog

## Unreleased

- 总部回复语音播放期间，短按上键可立即停止播放，且不会同时触发当前终端页面的导航操作。
- Passport 终端界面会跟随 TheGreatMe 中选定的语言。BLE 看板协议会同步 App 解析后的
  语言，终端无需重启即可在简体中文、繁体中文、英语、西班牙语、法语、德语、韩语和
  日语间更新；内嵌字库同时覆盖全部本地化终端标签，避免出现方框字。
- 在任意终端页面单击确定键可立即切换主题，双击确定键才进入设置页；设置页仍可用上键
  返回进入前的页面。
- Passport 目标进度条旁保留数字百分比，按 `7%`、`42%`、`100%` 自然显示，不补前导零。
- 总部回复改为可纵向滚动的文本视窗，长回复更新时自动跟随到最新一行；空闲时扩大
  可视区域，录音态临时收窄并保持滚动到底，避免底部文字被裁切或覆盖录音区。
- 协商支持后将 Passport 回复音频降为 8 kHz IMA ADPCM，同时保留 16 kHz 麦克风
  采集及旧版 16 kHz 扬声器音频块兼容；播放前先缓冲三个 100 ms 音频块，iPhone 再按
  稳定的 100 ms 节拍发送，减少长回复中的 BLE 队列断粮与溢出。
- Passport 回复语音默认使用满音量；对话页将带用户代号的发言放在右侧、总部回复放在
  左侧，并把录音波形收进用户一侧；短按上键返回任务主页，长按上键继续讲话；上下键
  长按触发录音时都会先播放同一段上扬提示音，提示音结束后再开麦，避免被录入并发送到
  iPhone。
- 普通断连与 Passport 重启后继续保留加密联动使用的 iPhone BLE 配对密钥；若 iOS
  已忘记自己一侧的配对，Passport 会只清除该手机的失效旧密钥，并在当前连接中继续
  新配对，避免陷入寻找、连接失败、断开后再次寻找的循环。
- 开机后直接进入“伟大的我”终端并立即启动 BLE，手机发现与重连不再依赖手动重新进入
  Demo 页面；仍可长按确定键返回硬件 Demo 菜单进行诊断。
- 将可连接广播对齐到标准 NimBLE 单过程 GAP 回调，使连接与订阅事件只有一个明确的
  处理入口。
- 在终端页右上角新增四格连接信号：iPhone 未连接时四格熄灭，BLE 的两条数据通道
  都就绪后显示满格信号。
- 将 Passport 对话页改成无边框终端对话：用户发言与 AI 回复以相邻命令行提示符呈现，
  识别、输入和语音播放状态直接显示在对话流中；移除“快速模式”“多轮上下文”等实现说明，
  按住说话的波形也融入终端，不再叠加第二张卡片；空状态只保留一行短提示，不再重复展示
  标签、占位文案和底部操作说明。
- 将联动页顶部简化为“伟大的我”“特工终端”两行品牌文案，移除首页中的固件版本号和
  右上角主题名称；版本与主题信息继续保留在设置页。
- 将联动服务以原生 16 位 `0xA2B0` UUID 放入 BLE 主广播包，并把设备名称移到扫描
  响应，修复错误 128 位字节序导致 CoreBluetooth 找不到 AI Passport、App 一直停在
  “正在寻找”的问题。
- 将 TheGreatMe 联动页重做为单色 CRT 终端：高密度状态分区、直角单像素分隔线和
  琥珀磷光；顶部改为与 iPhone 一致的目标卡，下方固定显示今日任务列表；短按上下键在
  今日任务间循环切换，并突出显示当前项。长按上键进入对话详情并开始按住说话，松开后发给
  iPhone 做 ASR，
  以快速模式完成默认支持多轮上下文的 AI 回复，再把文字和本机合成语音回传到 Passport
  扬声器。长按下键则进入 iPhone 首页原有的语音任务流程，自动分析每日最小胜利、最大
  阻碍和任务拆分。短按 OK 打开真正覆盖整屏的设置页；设置页中 OK 切换琥珀金、黑色、
  暖白、数码绿四套终端皮肤，上键返回；页面只保留连接状态、固件版本和主题选择，不再
  重复展示按键与 AI 模式说明。所有可见系统文案统一为简体中文，不再把中文数据与英文操作、
  状态提示混排。固件内置 1 位 16 px 简体中文字库，覆盖 GB2312 可解码的全部
  7,445 个字符，
  不再依赖 LVGL 内置的小型 CJK 子集。
- 将 BLE 示例升级为 TheGreatMe 联动页：安全连接 iPhone，同步真实目标与任务看板；
  通过带对话／任务路由标记的 IMA ADPCM 音频区分两种长按入口，并通过独立扬声器特征
  接收 AI 回复音频。音频帧缓冲移出任务栈，避免实机进入联动页时因栈溢出重启。
- 将小程序 BLE 安装兼容提升为二创模板强制契约：固定保护 `cardid`/Recovery 分区，
  保留上键持续 5 秒进入 Recovery 的 bootloader hook，并在 CI 强制校验合并镜像结构、
  分区表 MD5/范围、3 MB 应用上限和保护分区数据不入包。
- 规定多应用发布的 Release 标题约定：tag 按 `v<版本>-<应用名>`（如 `v0.1.0-voice-keychain`）命名，让 Release 标题同时带版本与应用名；发布成功后核对标题，保证一眼扫 Release 列表就能区分是哪个应用。
- 新增发布后收尾流程：`issue-suggestions` skill 用于把用户反馈作为 issue 提交到上游项目；`experience-pr` skill 用于把可复用的开发经验作为文档 PR 提交；新增 `docs/experiences/` 目录保存单条经验文件；并配套 `project-completion`、`file-issues` 与经验索引文档。
- 精简仓库根目录：将 GitHub 可识别的社区治理文档迁入 `.github/`，将变更记录迁入 `docs/`，同步全部引用，并在仓库检查中加入根目录文档白名单。
- 全仓库文档语言规范：所有维护中的 Markdown 默认 `.md` 文件使用英文，简体中文使用配对的 `.zh_CN.md`，双方提供语言切换；静态检查会阻止缺失配对、缺失切换链接或英文默认页混入中文正文。
- AI 开发流程一期：精简按任务加载的上下文入口，统一本地/CI 验证脚本，新增 PR 自动构建与模板，并提交依赖锁文件以提高构建可复现性。
- PR 审查修复：GitHub Actions 固定到完整 commit SHA，构建与发布 job 按最小权限拆分，同步 checkout 关闭凭证持久化；补充 Feature Request / Usage Question issue 表单；启用并修正私密安全报告兜底说明；清理 README 路径、CI 触发条件与历史分支描述漂移。
- 语言规范变更：commit 标题、PR 标题与 body 由"默认中文"改为**使用英文**（`docs/contribution/commit-and-pr.md` 更新）；中文写作规范（全角标点）适用范围剔除 PR/MR 描述（`doc-conventions.md` 更新）。
- CI 构建改造：`build-firmware.yml` 显式传入 `SDKCONFIG_DEFAULTS=sdkconfig.defaults` 再 `idf.py build`，由 defaults 启用自定义分区表（`CONFIG_PARTITION_TABLE_CUSTOM=y`，文件名为 `partitions.csv`）；`CONFIG_ESPTOOLPY_HEADER_FLASHSIZE_UPDATE` 改为 `n`，再用 `idf.py merge-bin -o build/FoloToy-AI-Passport-full.bin` 合并可直刷完整固件；产物精简为仅 full.bin；`actions/cache` 升级到 v5 以消除 GitHub Actions Node.js 20 弃用警告；CI 文档同步更新。
- 合并上游 PR #6（wireless-low-power-demos）以解决 PR #4 冲突：引入无线/低功耗 demo（`main/demo_wifi.c`、`demo_ble.c`、`demo_radio.c`、`demo_low_power.c`）、`partitions.csv`（NVS/PHY/3 MB factory-app 分区）、`main/CMakeLists.txt`/`main.c`/`demo.h`/`sdkconfig.defaults` 更新；同步硬件指南的 Wi-Fi/BLE/低功耗章节；README 能力契约表补充 Wi-Fi/Bluetooth LE/Low power 三项（中英双语）。
- 提交规范补充：`docs/contribution/commit-and-pr.md` 明确 PR 标题与 commit 标题使用相同的 Conventional Commit 格式和英文祈使句，不用名词短语当标题。
- CI 与文档清理：`sync-main.yml` 移除 `test_mode` 残留模板注释；`docs/development/coding-conventions.md` 将「Redis TTL」条目泛化为「缓存组件」条目（当前固件无 TTL 约束需求，消除从模板带入的无关约定）。
- 补充通用规范（借鉴 Shinku）：`docs/contribution/doc-conventions.md` 新增中文全角标点规范（正文 `，`；`（`）`，代码/命令/路径保留英文原样）、凭证不入仓规范（token/密钥/私钥绝不入仓，提交前 git diff 扫描敏感前缀）、文件删除安全规范（删除走系统回收站，不用 rm -rf/git clean -fd）。
- 代码注释规范强化：`docs/development/coding-conventions.md` 补充完善注释要求——函数说明（用途/参数/返回值/副作用/线程上下文/内存所有权/初始化顺序）、变量说明（语义/取值范围/生命周期/同步要求）、逻辑注释（状态机/时序/寄存器/魔数依据），覆盖范围宁多勿少，中文注释保留英文技术术语。
- 文档去 AI 化：`docs/README.md` / `docs/README.zh_CN.md` 移除 AI 专属章节（Entry point、Source-of-truth、提需求格式、BSP 边界、Runtime invariants、验收交付格式、构建命令），README 只保留给人看的项目介绍、硬件能力契约、demo 案例与项目结构；构建命令章节删除（与 `docs/development/build-and-test.md` 重复）。
- 新增 `docs/development/agent-guide.md`：集中承载"AI 如何在本仓库工作"（上下文建立顺序、事实来源优先级、提需求格式、BSP 边界、运行时规则、交付格式），并链接 build-and-test 与硬件指南，不重复构建命令与验收矩阵。
- 同步更新索引：`AGENTS.md` 规则索引新增 agent-guide 条目；`docs/INDEX.md` 与 `docs/development/README.md` 新增 agent-guide 索引行。
- 文档补充：`docs/fork-guide.md` 说明「为什么根目录不放置 README」——根目录 README 预留给 fork 开发者自行放置（上游留空），fork 后可将自己的内容写入根目录 `README.md` 介绍 fork 后的项目；GitHub 显示优先级（根 README > docs/README.md）契合该预留意图。
- 分支合并：创建 `main-update` 分支（基于与上游一致的 main），将 `feature/repo-structure`、`ci/build-firmware`、`ci/sync-main` 三个分支合并进来，统一 docs 结构（CI 文档归入 `docs/development/`，workflow 文件随 ci 分支引入 `.github/workflows/`）；解决 development/software-design README 的 add/add 冲突。
- 合并后审查修复：`docs/INDEX.md` 补充 CI 文档索引；`docs/fork-guide.md` 修正 workflow 引用为 `.github/workflows/sync-main.yml`；`docs/README` 双语项目结构块补充 `.github/workflows/` 与 CI 文档说明。
- ci 分支 CI 文档路径调整：`ci/build-firmware` 的 `docs/software-design/CI-build-and-release.md` 与 `ci/sync-main` 的 `docs/software-design/CI-sync-main.md` 均移入各分支的 `docs/development/`（CI 属工程规范）；`docs/software-design/README.md` 保留为软件设计索引；feature 分支的 software-design 索引同步更新引用。
- fork 补充文档目录迁移：`assets/docs/` 移至 `docs/assets/`（文档素材归入 docs/ 更合理），新增 `docs/assets/.gitkeep` 空目录占位；同步更新 AGENTS.md / INDEX / doc-conventions / fork-guide 的路径引用。
- 文档结构调整：根目录不再放 README——上游英文 README 移入 `docs/README.md`、中文移入 `docs/README.zh_CN.md`（GitHub 从 docs/ 识别主 README）；原 `docs/README.md` 根总索引更名为 `docs/INDEX.md`；同步更新 AGENTS.md / CONTRIBUTING / SUPPORT / fork-guide / doc-conventions 的路径引用。
- 初始化项目文档：新增 `AGENTS.md`、`CLAUDE.md` 和 `CHANGELOG.md`。
- 仓库结构规整：上游英文 `README.md` 更名为 `README.en_US.md`，保留 `README.zh_CN.md`。
- 新增目录骨架：`docs/`（software-design / hardware-design）、`assets/`（fonts / images / music，各含 `README.md`）、`skills/`。
- 将上游硬件开发指南归位到 `docs/hardware-design/AI_HARDWARE_DEVELOPMENT_GUIDE.md`。
- 文档规范：子目录 readme 统一为大写 `README.md`；补充 fork 用户约定（main 只动根 README）。
- 扩展 fork 用户约定：`main` 分支允许修改根目录 `README.md` 和 `assets/docs/`（README 不足以说明项目时存放补充文档与素材）。
- 新增 `assets/docs/` 目录约定：上游 main 只保留空目录 `.gitkeep`，内容文件仅存在于 fork；使用方法规范写入 AGENTS.md「给 fork 用户」约定。
- CI 文档迁移：`docs/software-design/CI.md` 从本分支移除，迁至 `ci/build-firmware` 分支并改名为 `docs/software-design/CI-build-and-release.md`。
- 补充 `main` 分支策略说明：解释 `main` 保持干净的两大原因（与上游同步无冲突 + 多小项目按分支整理）；例外——执意 main 开发需停用 CI 自动同步；提醒 fork 用户默认 action 关闭需手动启用（此条为整个 CI 的通用要求，统一写入 AGENTS.md）。
- 文档拆分：将 `AGENTS.md` 按主题拆为公共文档——新增 `docs/contribution/`（doc-conventions.md、commit-and-pr.md）与 `docs/development/`（build-and-test.md、coding-conventions.md），新增 `docs/fork-guide.md`；`AGENTS.md` 精简为简介 + 项目概述 + 必读文档索引。
- 同步更新索引：`docs/software-design/README.md`、`README.en_US.md` / `README.zh_CN.md` 的 `docs/` 目录说明。
- 参考 cindy 仓库文档组织完善索引：新增 `docs/README.md` 根总索引；AGENTS.md 规则索引按触发场景改写（附触发条件）；`docs/contribution/` 与 `docs/development/` 的 README 补充收录标准。
- 引入社区治理文档（参照 cindy 改写，放仓库根目录）：新增 `CONTRIBUTING.md` / `.zh_CN.md`（贡献指南，针对 ESP-IDF/AI agent/fork 场景改写）、`CODE_OF_CONDUCT.md` / `.zh_CN.md`（贡献者公约）、`SECURITY.md` / `.zh_CN.md`（安全报告流程）、`SUPPORT.md` / `.zh_CN.md`（支持渠道）；AGENTS.md 与 docs/README.md 同步引用。
