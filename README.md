# A demonstrating program that try to brute force from ETH private key to ETH address matching

Brute-forcing an Ethereum private key to find a matching Ethereum address is a highly inefficient and impractical approach due to the sheer size of the search space and the cryptographic strength of the Ethereum system. Ethereum private keys are 256-bit numbers, which results in a search space of 2^256 possible private keys. This makes brute-forcing infeasible with current technology.

However, I can provide a demonstration in code of how such a brute-force process would work theoretically, keeping in mind that this is purely educational and should never be used maliciously.

# How to run it

```
git clone git@github.com:gradchen/eth_brute_force.git
cd eth_brute_force
make
./eth_address_idx eth_addresses.txt
./eth_brute_force 10
```


# Runing output is like the following:
```
~/Downloads/eth_brute_force(main)$ ./eth_address_idx eth_addresses.txt
line 100000: 0xac23191aa95e88cb934dc746b6c9c91941b040fe
line 200000: 0xdeb70ad11ca4f6b27599f41ad2f71ce8dae584c8
line 300000: 0xef28121c1a266b6c05e154bbdc9928beb4a964ee
line 400000: 0xa21e5c9137012f3328af06627b1337b3e6b85d5d
line 500000: 0xb331aa229758e0686b501689c36a37f32b174d1a
line 600000: 0x62f63ae20a044b05c04dd5df7f33e63c34d34ef2
line 700000: 0x990a16233df3cd83fb0a2ff7abc2d28a03ba6bae
line 800000: 0x33888e29dc2aa3e0b8dc02741154592a6d1f65c9
line 900000: 0x72b47dfeee3c9876e8c2991b2bf3e71af9cf0fc7
line 1000000: 0x02d55789abdd48d5500fc9019507415f2a02d2ee

~/Downloads/eth_brute_force(main)$ ./eth_brute_force 10
Parent process 26471 waiting for Ctrl+C... to terminate ETH brute-force
[chidl 8] Tried: 100
[chidl 6] Tried: 100
[chidl 9] Tried: 100
[chidl 3] Tried: 100
[chidl 7] Tried: 100
[chidl 5] Tried: 100
[chidl 0] Tried: 100
[chidl 1] Tried: 100
[chidl 2] Tried: 100
[chidl 4] Tried: 100
[chidl 6] Tried: 200
[chidl 8] Tried: 200
[chidl 3] Tried: 200
[chidl 7] Tried: 200
[chidl 5] Tried: 200
[chidl 0] Tried: 200
[chidl 9] Tried: 200
[chidl 4] Tried: 200
[chidl 1] Tried: 200
[chidl 6] Tried: 300
[chidl 8] Tried: 300
[chidl 2] Tried: 200
......
^C
Parent received SIGINT. Terminating child processes...
Terminating child process 26472
Terminating child process 26473
Terminating child process 26474
Terminating child process 26475
Terminating child process 26476
Terminating child process 26477
Terminating child process 26478
Terminating child process 26479
Terminating child process 26480
Terminating child process 26481

Of course, the following matching results is fake (loool).
If you are lucky to get one, you are most lucky guy in this real world.
=======================================================================
~/Downloads/eth_brute_force(main)$ ls -l result_*.txt
-rw-rw-r-- 1 grady grady 38556 十二 22 18:23 result_0.txt
-rw-rw-r-- 1 grady grady 33684 十二 22 18:23 result_1.txt
-rw-rw-r-- 1 grady grady 37212 十二 22 18:23 result_2.txt
-rw-rw-r-- 1 grady grady 35700 十二 22 18:23 result_3.txt
-rw-rw-r-- 1 grady grady 36960 十二 22 18:23 result_4.txt
-rw-rw-r-- 1 grady grady 31584 十二 22 18:23 result_5.txt
-rw-rw-r-- 1 grady grady 39228 十二 22 18:23 result_6.txt
-rw-rw-r-- 1 grady grady 35616 十二 22 18:23 result_7.txt
-rw-rw-r-- 1 grady grady 40068 十二 22 18:23 result_8.txt
-rw-rw-r-- 1 grady grady 37212 十二 22 18:23 result_9.txt
```
