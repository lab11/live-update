import argparse
import os
import sys

parser = argparse.ArgumentParser(description='Generate app.conf file for app.')
parser.add_argument('app_dir', help='Application directory')
parser.add_argument('partition', type=int, help='App partition')
parser.add_argument('--no_app_relocation', action='store_true', help='Don\'t relocate application code')

args = parser.parse_args()

contents = \
"""
CONFIG_BOOTLOADER_MCUBOOT=y

CONFIG_UART_INTERRUPT_DRIVEN=y
CONFIG_FLASH=y

CONFIG_LIVE_UPDATE=n
CONFIG_LIVE_UPDATE_FUTURE=n

CONFIG_DEBUG=y
"""

relocation = \
"""
CONFIG_CODE_DATA_RELOCATION=y
CONFIG_HAVE_CUSTOM_LINKER_SCRIPT=y
CONFIG_CUSTOM_LINKER_SCRIPT="_build/app-sections.ld"

CONFIG_APP_FLASH_OFFSET={0}
CONFIG_APP_FLASH_SIZE={1}
CONFIG_APP_SRAM_OFFSET={2}
CONFIG_APP_SRAM_SIZE={3}
"""

print(contents)

partition_0_app_config = ['60000', '10000', '10800', '7c00']
partition_1_app_config = ['70000', '10000', '18400', '7c00']

if not args.no_app_relocation:
    if args.partition == 0:
        print(relocation.format(*partition_0_app_config))
    else:
        print(relocation.format(*partition_1_app_config))

# TODO parse existing application folder conf files and append, if necessary


