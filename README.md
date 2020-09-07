## 参考博文
拉流：https://blog.csdn.net/leixiaohua1020/article/details/46890487

推流：https://blog.csdn.net/leixiaohua1020/article/details/39803457

摄像头捕获：https://blog.csdn.net/leixiaohua1020/article/details/39702113

## 安装依赖
qt13
### MACOS
```asm
brew cask install xquartz
vcpkg install ffmpeg:x64-osx
```
### Ubuntu
```asm
vcpkg install ffmpeg:x64-linux
```

## 开发日志
* 2020-08/18 在mac上使用qt+ffmpeg完成拉流并且渲染视频
* 2020-08/22 完成摄像头捕获。添加一个管理类AVManager
* 2020-08/23 摄像头推流， 未完成

## TODO
- [x] 拉流 RTMP
- [x] 拉流并且渲染画面
- [ ] 拉流并且保存文件
- [ ] 拉流视频播放
- [ ] 拉流音频 + 视频播放
- [x] 摄像头捕获
- [ ] 推流摄像头数据
- [ ] 推流音频 + 视频的数据
- [ ] 播放本地文件