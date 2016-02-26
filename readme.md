# Pinocchio

Simple restful API push server for [Look and walk](http://www.lookandwalk.com/ko)

## Requirement

### Build
1. *cmake*
1. **boost**
1. **openssl** (for https)
1. tcmalloc (optional)

### Running
1. Redis server

## Platform

1. Mac OS X
2. Linux


## How to build

```
$> mkdir build
$> cd build
$> cmake ..
$> make
```
## How to run

```
$> cd src
$> ./pinocchio
```
## Configuration

* pinocchio.conf

Variable | Value Type | Default value | Description
---------|------------|---------------|------------
SERVER_IP | string |  0.0.0.0 | Server listen ip
SERVER_PORT | string | 18081 | Listen port
SERVER_NAME | string | Pinocchio/0.1 | Server name
AUTH_KEY | string | AIzaSyCaV3Ymy5IPqnxXVkrYmwG0IpB5mNj1ZZ8 | Application auth key
REDIS_IP | string | 127.0.0.1 | Redis server ip
REDIS_PORT | string | 6379 | Redis port
IS_SSL_SERVER | int | 0 | 1 -> https , 0 -> http
CERT_FILE_NAME | string | shyblue.sarang.net.crt | CERT file name for https server
KEY_FILE_NAME | string | shyblue.sarang.net.key | Key file name for https server

## API End Point

Alive check via http://server addreser:port/ will server return "This is pinocchio!" if server is alived.


End point|Param|Action
---------|-----|-------
/| | alive check
/regist/ | string under 128 byte | Add new user token
/gcm/send/ | Posted Json | Send push message to user
/gcm/recv/ | string user token | Get user's messages

## Test

* Add user

```
$> curl http://127.0.0.1:18081/regist/550e8400e29b41d4a71644665544000020151224110910
```
* Send messsage to user

```
$> sh send_msg.sh
```
* Receive messages of user

```
$> curl http://127.0.0.1:18081/gcm/recv/550e8400e29b41d4a71644665544000020151224110910

```