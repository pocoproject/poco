#!/bin/bash
bin/Linux/x86_64/HTTPFormServer &
sleep 1
curl --noproxy '*' -s http://127.0.0.1:9980 >/dev/null
curl --noproxy '*' -s http://127.0.0.1:9980 >/dev/null

terminated_at="$(date +%s)"
echo "Terminating HTTPFormServer"
pkill HTTPFormServer
wait

terminated_in="$(($(date +%s) - terminated_at))"
echo "HTTPFormServer took ${terminated_in} seconds to terminate"
