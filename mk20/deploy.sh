cd /Users/mick/dev/printrbot/display_phillip/display/Firmware/sketches/Teensy
platformio run
./hextobin.sh

cp firmware.bin /Users/mick/Dropbox/projects/printrbot/printrapp-server/public/firmware/mk20.bin

curl http://printrbot.local/updatemk20?url=http://192.168.1.135:3000/firmware/mk20.bin
#curl http://printrbot.local/updatemk20?url=http://10.0.0.39:3000/firmware/mk20.bin
