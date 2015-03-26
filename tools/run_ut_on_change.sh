#!/bin/bash

find . -regex '.*\.\(cpp\|h\|hpp\)' | entr ./tests.sh
