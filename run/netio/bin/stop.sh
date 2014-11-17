#!/bin/bash

NETIO="netio config=../config/netio.cfg"

kill -usr2 $(pidof $NETFRAME)

