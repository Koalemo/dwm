#!/usr/bin/env bash

rm -rf ~/workspaceconfig/DEPS/dwm

pushd ~/workspaceconfig

dbt sync --update --ignore-errors

pushd DEPS/dwm
sudo make clean install
popd
popd


