#!/bin/bash

pio run && \
\
platformio test --without-uploading --without-testing \
            --environment test_nano \
            --environment test_micro \
            --environment test_leonardo \
            --environment test_uno \
            --environment test_mega2560 && \
\
platformio test --environment test_native && \
\
platformio check --fail-on-defect=high --fail-on-defect=medium --fail-on-defect=low
