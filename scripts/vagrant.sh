#!/usr/bin/env bash

# Hacky: make sure we see gnuarm and/or west if it already exists
export PATH=/usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin:"$PATH"
export PATH=/usr/local/bin:"$PATH"

# Setup script for Vagrant build
apt-get update
apt-get install -y g++

# Install CMake v3.15.5, if it's not there already
if [[ ! $(cmake --version) =~ "3.15.5" ]]
then
    pushd .
    cd /tmp
    wget https://github.com/Kitware/CMake/releases/download/v3.15.5/cmake-3.15.5.tar.gz
    tar -xzvf cmake-3.15.5.tar.gz
    cd cmake-3.15.5
    ./bootstrap
    make -j4
    make install
    popd
else
    echo "CMake already installed:"
    cmake --version
fi

# Install arm-eabi-none toolchain, if it's not there already
if [[ ! $(arm-none-eabi-gcc --version) =~ "9-2019-q4-major" ]]
then
    pushd .
    cd /usr/local
    wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/RC2.1/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2
    tar -xvjf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2
    echo 'export PATH=/usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin:"$PATH"' >> /home/vagrant/.bashrc
    popd
else
    echo "arm-none-eabi-gcc already installed:"
    arm-none-eabi-gcc --version
fi

# Install python libraries
apt-get install -y python3-pip
pip3 install pycryptodome

# Zephyr setup
apt-get install -y --no-install-recommends \
git cmake ninja-build gperf ccache dfu-util device-tree-compiler wget \
python3-pip python3-setuptools python3-tk python3-wheel xz-utils file make \
gcc gcc-multilib

if ! west --version
then
    pip3 install west
    echo 'export PATH=/usr/local/bin:"$PATH"' >> /home/vagrant/.bashrc

    pushd .
    cd /vagrant/zephyros
    west init
    west update
    popd
fi

pip3 install -r /vagrant/zephyros/zephyr/scripts/requirements.txt

if [[ ! $(cat /home/vagrant/.bashrc) =~ "ZEPHYR_TOOLCHAIN_VARIANT" ]]
then
    echo 'export ZEPHYR_TOOLCHAIN_VARIANT=gnuarmemb' >> /home/vagrant/.bashrc
fi

if [[ ! $(cat /home/vagrant/.bashrc) =~ "GNUARMEMB_TOOLCHAIN_PATH" ]]
then
    echo 'export GNUARMEMB_TOOLCHAIN_PATH=/usr/local/gcc-arm-none-eabi-9-2019-q4-major' >> /home/vagrant/.bashrc
fi

if [[ ! $(cat /home/vagrant/.bashrc) =~ "ZEPHYR_BASE" ]]
then
    echo 'export ZEPHYR_BASE=/vagrant/zephyros/zephyr' >> /home/vagrant/.bashrc
fi

if [[ ! $(cat /home/vagrant/.bashrc) =~ "ZEPHYR_SDK_INSTALL_DIR" ]]
then
    wget -O /tmp/sdk_setup.run https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.10.3/zephyr-sdk-0.10.3-setup.run
    chmod u+x /tmp/sdk_setup.run
    /tmp/sdk_setup.run -- -d /usr/local/zephyr_sdk
    echo 'export ZEPHYR_SDK_INSTALL_DIR=/usr/local/zephyr_sdk' >> /home/vagrant/.bashrc
    echo 'export PATH=/usr/local/zephyr_sdk/sysroots/x86_64-pokysdk-linux/usr/bin:"$PATH"' >> /home/vagrant/.bashrc
fi

# USB device rules
wget -O /tmp/60-openocd.rules https://sf.net/p/openocd/code/ci/master/tree/contrib/60-openocd.rules?format=raw
cp /tmp/60-openocd.rules /etc/udev/rules.d
udevadm control --reload

if [[ ! $(cat /home/vagrant/.bashrc) =~ "zephyr-env.sh" ]]
then
    echo 'source /vagrant/zephyros/zephyr/zephyr-env.sh' >> /home/vagrant/.bashrc
fi

pip3 install pyelftools
apt-get install -y srecord python-serial

#addgroup vboxusers
#adduser vagrant vboxusers
#mkdir /media/musca
#mount /dev/sdb /media/musca
