# 前端目录（Vue3 + TypeScript + Pinia）
src/
├── api/            # API 接口定义层（只写 URL 和 method，每个业务模块一个文件）
├── mock/
│   ├── index.js        # 路由映射表 (mockDataMap)，所有 Mock 路由在此注册
│   ├── interceptor.js  # axios 拦截器（Mock 核心开关）
│   └── modules/        # Mock 数据实现，与 api/ 一一对应
├── stores/         # Pinia 状态层（可选，用于跨组件共享数据，内部仍调用 api/）
├── views/          # 页面
├── components/     # 公共组件（图表、顶栏、面板等）
├── router/
│   └── index.ts    # 路由配置（含权限守卫）
└── utils/
    └── request.ts  # axios 封装（含 Mock 拦截接入点）

# 后端目录（SpringBoot DDD 分层，包名以 [基础包名] 为前缀）
[基础包名]/
├── application/
│   ├── adapter/
│   │   ├── api/                        # REST 入口（XxxAppService）
│   │   ├── consumer/                   # MQ 消费者
│   │   └── scheduler/                  # 定时任务（XxxTask）
│   ├── service/
│   │   ├── XxxAppService.java          # 应用服务
│   │   └── process/
│   │       └── XxxProcess.java         # 编排流程
│   ├── repository/                     # 仓储接口（IXxxAppRepository）
│   ├── dto/                            # 请求/响应体（XxxReqDTO / XxxRespDTO）
│   ├── converter/                      # 转换器（XxxAppConverter）
│   ├── external/                       # 外部服务接口（IXxxExternalService）
│   └── error/                          # 错误码（AppErrorEnum）
├── domain/
│   └── [聚合根]/
│       ├── entity/                     # 实体（无后缀）/ 值对象（XxxVO）
│       ├── event/                      # 领域事件（完成时态，XxxEvent）
│       ├── repository/                 # 仓储接口（IXxxRepository）
│       ├── constants/                  # 常量（XxxConstants）
│       ├── enums/                      # 枚举（XxxEnum）
│       ├── error/                      # 错误码（XxxErrorEnum）
│       └── properties/
├── infrastructure/
│   └── [聚合根]/
│       └── repository/
│           ├── dao/                    # DAO（XxxDao）
│           ├── cache/                  # 缓存（XxxCache）
│           ├── storage/                # 对象存储（XxxStorage）
│           ├── dataObject/             # 持久化对象（XxxDO）
│           └── converter/              # 转换器（XxxConverter）
│               └── XxxRepository.java  # 仓储实现（XxxRepository）
├── external/
│   └── [外部中心名称]/
│       ├── dto/                        # 外部 DTO（XxxReqDTO / XxxRespDTO）
│       ├── converter/                  # 防腐转换（XxxConverter）
│       ├── feign/                      # Feign 客户端（IXxxFeignClient）
│       ├── error/                      # 错误码（ExternalErrorEnum）
│       └── properties/
│           └── XxxExternalService.java # 外部服务实现
└── boot/                               # 启动层（包名固定：[基础包名].boot）
    └── XxxApplication.java             # 启动类
