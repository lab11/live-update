pkill -f JLink
osascript -e 'tell application "Terminal" to do script "JLinkExe -device NRF52832_XXAA -USB 682725125 -if swd -speed 4000 -Autoconnect 1"'
sleep 1
osascript -e 'tell application "Terminal" to do script "JLinkRTTClient > write_0909_x10_e2e.csv"'
sleep 1
python3 ../../../scripts/repeat_update.py --write_only --niter 10 ../ /dev/cu.usbmodem0009600393215 ../update ../original

