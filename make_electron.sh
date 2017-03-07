#win x86
#node-gyp rebuild --target=1.4.15 --arch=ia32 --dist-url=https://atom.io/download/electron

#mac x64
rm -rf build
node-gyp rebuild --target=1.4.15 --arch=x64 --dist-url=https://atom.io/download/electron

./link.sh

cp build/Release/screencapture.node ~/work/electron/electron-quick-start/
