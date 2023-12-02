#!/bin/sh

echo "#pragma once"
echo ""
echo "#include <string_view>"
echo "#include <vector>"
echo ""
echo "const std::vector<std::string_view> input = {"
cat $1 | awk '{print "    \"" $0 "\","}'
echo "};"
