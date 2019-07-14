#!/bin/bash

sudo -E gdb -tui attach $(pgrep seamail)
