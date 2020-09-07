## Pacemaker Testing

### Compiling original and VRP update code

1. First, ensure you're at the right place: `cd <project root>/apps/pacemaker` and clean out any old builds: `make clean`

2. [Vagrant] Compile the original code. This will generate a `_build/update` folder containing everything we need for the update process, which is copied while we compile the updated code:

```
cp testing/no_vrp_main.c src/main.c
make
rm -rf original/
cp -r _build/update original/
```

I like to && all of these commands together into one command I can run multiple times if necessary.

3. [Host] Flash the original code version to the board with `make flash`.

3. [Vagrant] Now compile the updated code folliwnng the same process. The update process requires an additional piece of Python code to do custom state transfer and hardware initialization, which is included in the update folder.

```
cp testing/updated_vrp_main.c src/main.c
make
rm -rf update/
cp -r _build/update update
cp testing/custom_transfer.py update/
```

The result is two folders, `original` and `update`, that contain all the information necessary for a live update that includes the VRP pacemaker component.

### Compiling and flashing the nRF52 heart model code

1. The nRF52 runs the heart model, which simulates the atrial and ventricle sense events to the pacemaker, running on the nRF91. To compile this code, make sure you're in the testing folder with `cd <project root>/apps/pacemaker/testing`.

2. [Host] `make flash`. If a device picker pops up, make sure you're flashing to the nRF52.

### Test Setup

Connect the following pins between the nRF9160 pacemaker and the nRF52832 testing board:

```
   52832 (Testing)  <-->  9160 (Pacemaker)

VENTRICLE_SENSE 25        10             
ATRIAL_SENSE    24        11
VENTRICLE_PACE  23        12
ATRIAL_PACE     22        13

UPDATE_FINISHED 18        18
WRITE_FINISHED  19        19
```

Make sure the nRF9160 is set to 3.3V VDD IO (it's a tiny switch along the bottom edge of the board).

### Manual update

[Host] You can use the update script to manually trigger an update:

```
python3 <project root>/scripts/update.py <project root>/apps/pacemaker /dev/cu.usbmodem0009600...5 --flashed_folder <project root>/apps/pacemaker/original --update_folder <project root>/apps/pacemaker/update --no_update_flashed
```

The `flashed_folder` and `update_folder` flags point the updater to the versions we compiled above, and `no_update_flashed` tells the updater to forget about the update, so you can reset the board and run it again without running into version/partition issues.

### Repeated updates

[Host] Running one update at a time is annoying and time-consuming, so it's normally a lot easier to run updates one after another for evaluation purposes. The script will reset both test and pacemaker boards in the correct order, let the pacemaker run for a couple of seconds, then randomly apply the update and wait a couple more seconds. The entire process is repeated as many times as necessary:

```
python3 <project root>/scripts/repeated_update.py --niter <number of runs> <project root>/apps/pacemaker /dev/cu.usbmodem0009600...5 ../update ../original
```

Additional flags are available to test different scenarios (e.g. write-only updates, predicate checking only, full update, etc.)

### End-to-end testing

[Host, Mac-only] The `*_e2e_test.sh` scripts automate the process of running repeated updates and saving RTT console output to a file. They can be modified to tune the number of runs, or use different flags per-test. An template/example is provided below:

```
#!/bin/bash

pkill -f JLink
osascript -e 'tell application "Terminal" to do script "JLinkExe -device NRF52832_XXAA -USB <serial number> -if swd -speed 4000 -Autoconnect 1"'
sleep 1
osascript -e 'tell application "Terminal" to do script "JLinkRTTClient > <output file path>"'
sleep 1
python3 <project root>/scripts/repeated_update.py --dry_run --niter <number of runs> <project root>/apps/pacemaker /dev/cu.usbmodem0009600...5 ../update ../original
```

### Generating rough figures

By default, `testing/main.c` should output AS-VP response times in a CSV format, which you can use the `summarize_results.py` script to parse, summarize, and generate rough box-and-whisker figures. For example, given a set of `normal` execution data (i.e. without an update occuring) and `update` data:

```
python3 <project root>/apps/pacemaker/testing/summarize_results.py --normal_csv <path to normal data> --update_csv <path to update data>
```

The resulting figure should show up as `FIGOUT...`.
