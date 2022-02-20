将用于 OJ judger的沙盒

# 提供的服务
仅编译/运行用户提交的代码。并返回结果。

使用时直接调用可执行文件，通过管道注入输入输出文件路径。

# 提供的隔离:
## Linux namespaces 的隔离。
UTS (hostname), MOUNT (chroot), PID (separate PID tree), IPC, NET (separate networking context), USER, CGROUPS

## 文件系统的限制：
FS constraints: chroot(), pivot_root(), RO-remounting, custom /proc and tmpfs mount points

## 资源的限制：
Resource limits (wall-time/CPU time limits, VM/mem address space limits, etc.)

## 系统调用的隔离：
Programmable seccomp-bpf syscall filters (through the kafel language)

# 参数
|传入参数| 字段名称 | 含义 |
|---- | ---- | ---- |
|time|time_limit_in_ms| 时间限制(ms) |
|mem|memory_limit_in_byte| 空间限制(byte)|
|inDir|input_directory_to_mount| 传入的文件所在的文件夹（由于资源隔离，需要挂载）|
|outDir|output_directory_to_mount| 传出的目标文件所在的文件夹 |
|command|command| 需要在容器内执行的指令 |
