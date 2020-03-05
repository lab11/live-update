git rev-parse HEAD > /tmp/liveupdate-git-rev
rm -rf .update/$(< /tmp/liveupdate-git-rev)/
mkdir .update/$(< /tmp/liveupdate-git-rev)/
cp zephyros/zephyr/build/zephyr/zephyr.map zephyros/zephyr/build/app/relocated_pic.elf .update/$(< /tmp/liveupdate-git-rev)/

