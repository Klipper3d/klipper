#!/bin/bash
# This scripts cleanups mkdocs output before commit

# Avoid push of pycache
find "${PWD}/site/" -type f -name "*.pyc" -delete
