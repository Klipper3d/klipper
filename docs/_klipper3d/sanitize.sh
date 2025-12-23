#!/bin/bash
# This scripts cleanups mkdocs output before commit

# Avoid push of pycache
find "${PWD}/site/" -type f -name "*.pyc" -delete
find "${PWD}/site/" -type f -name "sitemap.xml.gz" -delete
find "${PWD}/site/" -type f -name "sitemap.xml" -delete
