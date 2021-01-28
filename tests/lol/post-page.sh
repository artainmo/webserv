#!/bin/bash

echo "open $1 $2"
sleep 2
echo "POST $3 HTTP/1.1"
echo "Host: $1"
echo "Content-type: test/file"
echo "Content-length: 16"
echo "Transfer-Encoding: chunked"
echo
echo "10"
echo "<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>
 <?php echo '<p>Hello World</p>'; ?>
 </body>
</html>"
echo "0"
#echo "\r\n"
echo
echo
sleep 2