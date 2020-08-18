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

## TODO
- [ x ] 拉流

- [ x ] 推流

- [ x ] 拉流音视频播放

- [ x ] 播放本地文件