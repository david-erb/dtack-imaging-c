#!/bin/sh
rm -f ETAGS
find . -name '*.[ch]' -exec etags --append -o ETAGS {} \;