# Benchmarking
This folder is used in order to benchmark cpphttp.

## Before you begin

Make sure to install packages mentionned in the main [README](../../README.md)\
Then install ab from apache on your computer.\
For example on Ubuntu/Debian you can do :
```bash
sudo apt-get install apache2-utils
```

## Building the server

```bash
mkdir -p build && cd build && cmake .. && make
```

## Start the server

In the build folder just do :
```bash
./simple_server
```

## Start the benchmark
While the server is running in another terminal do :
```bash
./benchmark.sh
```
You can modify this file if you want to do more requests, change request type, and so on...

## Expected result

On my machine, with a single-threaded server, you can have this kind of result :

```
Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8080

Document Path:          /
Document Length:        68 bytes

Concurrency Level:      1000
Time taken for tests:   0.748 seconds
Complete requests:      10000
Failed requests:        0
Keep-Alive requests:    10000
Total transferred:      1310000 bytes
HTML transferred:       680000 bytes
Requests per second:    13373.51 [#/sec] (mean)
Time per request:       74.775 [ms] (mean)
Time per request:       0.075 [ms] (mean, across all concurrent requests)
Transfer rate:          1710.87 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    3  10.0      0      48
Processing:     0   66  13.1     70      76
Waiting:        0   66  13.1     70      76
Total:          0   69   5.9     70      77

Percentage of the requests served within a certain time (ms)
  50%     70
  66%     70
  75%     70
  80%     70
  90%     71
  95%     72
  98%     75
  99%     76
 100%     77 (longest request)
 ```
