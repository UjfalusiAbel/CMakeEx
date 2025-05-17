#!/bin/bash

gnome-terminal -e bash -c "./start_python.sh; exec bash" &
sleep 4
gnome-terminal -e bash -c "./start_cpp.sh; exec bash" &