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

## API End Point

Alive check via http://server addreser:port/ will server return "This is pinocchio!" if server is alived.
