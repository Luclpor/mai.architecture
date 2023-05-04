                                                      1 threads and 1 connections
No cache 
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.24ms    1.32ms  14.99ms   94.94%
    Req/Sec     0.95k   183.16     1.41k    81.82%
  Latency Distribution
     50%    0.99ms
     75%    1.29ms
     90%    1.64ms
     99%    7.66ms
  1039 requests in 1.10s, 335.85KB read
  Cache
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   735.83us  677.40us  10.25ms   97.04%
    Req/Sec     1.47k   140.56     1.67k    70.00%
  Latency Distribution
     50%  637.00us
     75%  837.00us
     90%    1.05ms
     99%    3.47ms
  1470 requests in 1.00s, 485.21KB read

                                                        2 threads and 2 connections
NoCache
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.01ms  612.02us   7.81ms   84.92%
    Req/Sec     1.04k   246.15     1.46k    62.50%
  Latency Distribution
     50%    0.90ms
     75%    1.21ms
     90%    1.59ms
     99%    3.59ms
  4134 requests in 2.01s, 1.27MB read
Requests/sec:   2061.07
Cache
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   697.85us  672.31us  10.99ms   94.87%
    Req/Sec     1.58k   233.79     2.10k    69.05%
  Latency Distribution
     50%  591.00us
     75%  834.00us
     90%    1.11ms
     99%    3.26ms
  6615 requests in 2.10s, 2.05MB read
Requests/sec:   3149.79

                                                          5 threads and 5 connections
No cache
Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.41ms    0.92ms  14.31ms   83.78%
    Req/Sec   746.79    138.09     1.09k    70.40%
  Latency Distribution
     50%    1.22ms
     75%    1.73ms
     90%    2.40ms
     99%    4.74ms
  18599 requests in 5.01s, 5.88MB read
Requests/sec:   3713.78
Cache
Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.07ms    0.91ms  13.99ms   91.69%
    Req/Sec     1.03k   235.15     1.72k    66.40%
  Latency Distribution
     50%    0.88ms
     75%    1.30ms
     90%    1.83ms
     99%    4.64ms
  25621 requests in 5.01s, 8.03MB read
Requests/sec:   5115.80

                                                            10 threads and 10 connections
NoCache
Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     3.02ms    1.98ms  30.21ms   80.15%
    Req/Sec   350.73     68.75   585.00     68.30%
  Latency Distribution
     50%    2.61ms
     75%    3.80ms
     90%    5.30ms
     99%    9.88ms
  34969 requests in 10.02s, 11.03MB read
Requests/sec:   3488.87
Cache
Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.95ms    1.41ms  25.71ms   80.20%
    Req/Sec   547.78     93.57     0.92k    69.00%
  Latency Distribution
     50%    1.71ms
     75%    2.50ms
     90%    3.50ms
     99%    6.67ms
  54582 requests in 10.02s, 16.90MB read
Requests/sec:   5446.00
