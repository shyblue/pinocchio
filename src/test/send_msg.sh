#!/bin/sh

curl --header "Authorization: key=AIzaSyCaV3Ymy5IPqnxXVkrYmwG0IpB5mNj1ZZ8" \
     --header Content-Type:"application/json" -k https://127.0.0.1:18081/gcm/send \
     -d "{ \"data\":{\"board_id\":\"1234\",\"board_seq\":\"1\" },\"to\":\"550e8400e29b41d4a71644665544000020151224110910\"} "
