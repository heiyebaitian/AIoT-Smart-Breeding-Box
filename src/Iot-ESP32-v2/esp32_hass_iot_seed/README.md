# esp32_hass_iot_seed

 基于Aiot-NLP的智能育种箱 代码,该存储库包含了ESP32S3控制器代码及Home Assistant(下面简称HASS)配置文件。
 该代码无法独立使用,详细请见[GitHub:AIoT-Smart-Breeding-Box](https://github.com/heiyebaitian/AIoT-Smart-Breeding-Box)获取项目完整内容。

## 功能介绍

本项目基于 **HASS** 平台重写,添加了许多v1版本不支持的功能,并改进修复了v1版本存在的大量问题
项目采用主流Iot服务框架HASS,配合MQTT通讯协议实现,您在复现本项目时可能需要完成HASS和MQTT代理服务器的搭建,由于篇幅有限,本文档不再对此过程进行描述。

### 全新设计的仪表盘界面

基于HASS框架的现代化UI风格全新设计的界面,适合触屏、鼠标点击等多种方式交互,支持不同屏幕大小的自适应缩放适配,多主题配色自定义支持,提供用户友好型交互和客制化风格体验,拥有比上一代更加优秀的人机交互体验。

![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/home-1.png)
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/home-2.png)
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/home-phone.png)

### 优雅的动效与图表数据呈现
基于WebUI设计的控件为智能化育种管理平台带来了全新流畅的优雅动效设计与丰富的控件功能,数据可以不再以单一的数字方式呈现,而是能够成为图表控件的一部分,让数据人工分析处理更加直观高效便捷。

![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/chart-1.png)
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/chart-2.png)

### 多育种箱集群管理支持
使用区域功能可以对分布在不同位置的育种箱集中化管理,快速下发统一的育种任务指令以及收集汇总育种数据,不再需要用户一台台设备的重复操作,通过远程化管理可以实现现场无人值守工作。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/regionalism.png)

### 多地育种基地集中化管理
由HASS提供的多地管理支持,可以远程集中化管理其他地点的设备,查看设备采集数据内容。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/map.png)

### 权限管理与日志审计
基于HASS平台的多用户管理支持,可以对不同的用户进行分组和权限管理,有效确保数据安全性及合规性,为智能化育种平台数据安全保驾护航。
完善的日志审计功能,可以方便管理员对设备运行状况进行高效调试,以及发生事故时的问题调查工作展开。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/users.png)
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/log.png)

### 丰富的数据面板
数据面板可以帮助使用者快速了解育种设备的近期运行状况及收集的环境数据,帮助使用者便捷分析数据内容。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/history.png)

### 待办事项提醒
待办事项提醒可以帮助用户快速了解近期需要处理的事物,提供事物概览,提高工作效率,同时系统会自动根据近期的运行状况和收集的育种环境数据等内容在待办事项内给出智能建议,帮助用户高效使用育种平台,充分加强人机协作效率。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/backlog.png)

### 人机交互友好的自动化编程界面
用户无需了解复杂的编程语言即可使用自然语言逻辑进行自动化流程设计,对育种箱进行快速重新编程,通过简单模块化拼接即可完成自动化任务创建操作,大大减轻了用户学习成本,同时提供了更多操作可能,实现高效的人机协作。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/automation-1.png)
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/automation-2.png)
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/automation-3.png)

### 自定义API接口
当用户使用其他软件系统时,可以通过平台内置的API接口对各个功能对象实现快速调用,轻松完成二次开发操作,便于实现多平台融合使用,提高人机协作效率。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/custom.png)

### 高安全性支持
动态口令 TOTP(Time based One Time Password,30 秒一变的动态口令)是市场中最通用的、经济的安全认证方式,平台支持使用TOTP以及其他传统验证方式,提高账户安全性。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/authenticator.png)

### 高可用性支持
基于容器化和集群技术打造的后端服务支持,为智能育种平台提供了高可靠性支持,对意外风险的防范能力大幅增强,通过冗余备份,可以有效降低软件崩溃、意外断电、网络崩溃等情况的数据丢失和服务中断风险。
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/panel-1.png)
![](https://github.com/heiyebaitian/esp32_hass_iot_seed/blob/main/public/image/panel-2.png)