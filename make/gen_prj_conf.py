import argparse
import os
import sys

parser = argparse.ArgumentParser(description='Generate app.conf file for app.')
parser.add_argument('app_dir', help='Application directory')
parser.add_argument('--no_app_relocation', action='store_true', help='Don\'t relocate application code')

args = parser.parse_args()

contents = \
"""
CONFIG_UART_INTERRUPT_DRIVEN=y
CONFIG_GPIO=y
CONFIG_LIVE_UPDATE=y
"""
#CONFIG_LIVE_UPDATE_FUTURE=y

relocation = \
"""
CONFIG_CODE_DATA_RELOCATION=y
CONFIG_HAVE_CUSTOM_LINKER_SCRIPT=y
CONFIG_CUSTOM_LINKER_SCRIPT="_build/app-sections.ld"
"""

print(contents)
if not args.no_app_relocation:
    print(relocation)

# TODO parse existing application folder conf files and append, if necessary


