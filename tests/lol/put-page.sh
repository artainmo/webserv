#!/bin/bash

echo "open $1 $2"
sleep 2
echo "PUT $3 HTTP/1.1"
echo "Host: $1"
echo "Content-type: text/html"
echo "Content-length: 16"
echo
echo "<p>New File</p>"
#echo "\r\n"
echo
echo
sleep 2
