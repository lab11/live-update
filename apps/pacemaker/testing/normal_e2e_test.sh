pkill -f JLink
osascript -e 'tell application "Terminal" to do script "JLinkExe -device NRF52832_XXAA -USB 682725125 -if swd -speed 4000 -Autoconnect 1"'
sleep 1
osascript -e 'tell application "Terminal" to do script "JLinkRTTClient > normal_0904_x50_e2e.csv"'
sleep 1
python3 ../../../scripts/repeat_update.py --dry_run --niter 50 ../ /dev/cu.usbmodem0009600393215 ../update ../original

