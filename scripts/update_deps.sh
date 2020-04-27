git submodule update --init --recursive

cd ext/zephyros && west update && cd -
cd ext/zephyros/zephyr && git pull && cd -

cd ext/ncs && west update && cd -
