## Live Updates

Note: repo under construction


#### nRF9160 Setup Instructions

(in Vagrant)
1. Delete any contents in your `/vagrant/ext/ncs` directory
2. `$ cd /vagrant/ext/ncs`
3. `$ west init -m https://github.com/jlwatson/sdk-nrf`
4. `$ west update`
5. `$ west zephyr-export`
6. Install the zephyr dependencies (just to make sure)
```
pip3 install -r zephyr/scripts/requirements.txt
pip3 install -r nrf/scripts/requirements.txt
pip3 install -r bootloader/mcuboot/scripts/requirements.txt
```
7. Update your `~/.bashrc` with:
```
export ZEPHYR_BASE=/vagrant/ext/ncs/zephyr
source $(ZEPHYR_BASE)/zephyr-env.sh
```
8. Ensure that your app Makefile sets `BOARD = nrf9160` and run `make clean` before building.
