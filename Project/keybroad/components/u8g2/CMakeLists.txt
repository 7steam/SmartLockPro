#idf_component_register(SRCS "u8g2.c"
#                   INCLUDE_DIRS "include")
# 定义变量，存储所有 .c 文件路径
file(GLOB MY_C_FILES "src/*.c")  # "src/" 是目标目录，"*.c" 匹配所有 .c 文件
## 递归匹配 "src" 目录及所有子目录下的 .c 文件
#file(GLOB_RECURSE MY_C_FILES "src/*.c")
# 在 ESP-IDF 中，将文件添加到组件编译（如果是 main 组件）
idf_component_register(
    SRCS ${MY_C_FILES}  # 通过变量引用所有 .c 文件
    INCLUDE_DIRS "include" "src" 
    REQUIRES driver  # 包含目录（根据实际情况修改）
)
# 排除 "src/test" 目录下的 .c 文件
#file(GLOB_RECURSE MY_C_FILES 
#    "src/*.c"
#   PATTERN "src/test/*" EXCLUDE  # 排除指定路径
#)
