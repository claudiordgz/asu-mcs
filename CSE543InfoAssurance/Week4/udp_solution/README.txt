# Thought Process to Steal the Flag

Initially, my thought process was:

  1. Craft a UDP Packet with the trusted IP as the SRC IP
  2. Send it to flagserv.cse543.rev.fish:13337
  3. Wait for a response

This approach was incorrect. 

After multiple tries searching in tcpdump, I thought the answer was a "log finding" issue and installed Wireshark. But, unfortunately, I couldn't find the flag.

The correct solution is in the Overview Document in the following statement:

> If a user's source IP address is trusted, FlagServ will send out the flag to the specified destination (repeat: via UDP). 

I corrected my thought process to the following:

  1. Craft a UDP Packet with the trusted IP as the SRC IP.
  2. Get the IP Address of the VPN Network Interface, and add it as a Packet payload.
  3. Log traffic on the VPN Network Interface. (In my case, I used Wireshark on Ubuntu)
  4. Send packet to flagserv.cse543.rev.fish:13337, source port didn't matter. 
  5. Analyze packets for the message (which is not a response, it is a UDP message)

After this, I was quickly retrieving the message from the logged data in Wireshark.

I saved the file file as a *.pcapng from Wireshark and made note of the row with the Packet.

I opened Scapy in the terminal and did the following:

```
p = rdpcap("/path/to/file.pcapng")
raw(p[-1])
```

Copy pasted the Load content from the Packet, saved it to a flag.txt. And uploaded.

# Lessons Learned

0. Read the problem multiple times.
1. Make sure to know how to filter log information before starting. 
    - What network to filter by?
    - What port/source ip?
2. Take note of from, what, and where are messages going to, and include the network interfaces.
3. Make code, and run. 

# Instructions

Install and run using the following:

python3 -m venv .env
. ./.env/bin/activate
pip3 install -r requirements.txt

Then go to wireshark to "tap0" and start recording packets.

Then run: sudo python3 ./main.py -u -v

Then look in wireshark for packet.

