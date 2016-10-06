cd /Users/mick/Dropbox/projects/printrbot/printr-client/printrhub_esp
platformio run
#cp .pioenvs/nodemcuv2/firmware.bin /Users/mick/Dropbox/projects/printrbot/printrapp-server/public/firmware/
cp .pioenvs/esp12e/firmware.bin /Users/mick/Dropbox/projects/printrbot/printrapp-server/public/firmware/
curl http://printrbot.local/updatefirmware?url=http://192.168.1.68:3000/firmware/firmware.bin
#curl http://printrbot.local/updatefirmware?url=http://10.0.0.39:3000/firmware/firmware.bin
