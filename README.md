# My Hacking Tools
Different tools developed by me that can be used for hacking

## How to Compile Everything?

You can use *cmake* if you are used to use it or you can use *cmake.sh* shell script to do it automatically. It will create *build* folder with all the executables.

## macchanger

It is my own implementation of macchanger included in Kali Linux distro.

Firt of all, you must shutdown the interface you are going to change the mac address typing ``sudo ifconfig iface down``, where *iface* is the interface name. Later, you can use *macchanger* and, to conclude, ``sudo ifconfig iface up`` to enable the interface again. You will see that it's ethernet address has ben changed.

### Usage:

In *build* folder:
```
$ ./macchanger [-h] device [random] [mac_addr]
```

* **-h:** Show help.

* **device:** interface name whose mac address you want to change. For example: *wlan0*.

* **random:** if you use this flar (literally), *macchanger* program, will generate a random mac address. Use it instead of *mac_addr*.

* **mac_addr:** mac address yo want to use. use it instead of *random*
