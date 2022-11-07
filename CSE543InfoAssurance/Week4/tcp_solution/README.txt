# Thought Process to Steal the Flag

This homework was tough. I read the problem many times and tried to predict the Sequence number, and no genuine attempts were made for days while trying to find the pattern. 

I tried using the packet.seq to find a pattern for days, graphing it in excel, and getting lists, all to no avail. After reading all these talks about permutations on Slack, it made me more confused. Eventually, I looked into the raw sequence in Wireshark and the hex symbols and started seeing the pattern everyone was talking about in Slack.

Talking with a teammate, we started assembling the permutations. But something about trying 24 calls to get one right in the ACK phase didn't sound right to me. So I started debugging the packets and the time they came in until I noticed the secret.

The packet.time would be the sequence time now and then. So I started calculating all permutations of time.time() into hex and could find the sequence for every call in my permutations. Instead of attempting to get the ACK call right, I decided to get the whole thing right or nothing. If there are 1/24 chances the time.time() is the sequence, I only need 24 tries to get it once. 

I assembled the whole thing and went back to the video. I ensured my packet_a_seq on the A packet was packet_s_seq + 1 from the S packet. I also saved time_before_sending_S_packet = time.time() right before sending the S packet, and assumed that was the sequence the server would use. I used that time_before_sending_S_packet + 1 as my ack for my A packet. I recalculated the checksum right before sending it. It would still not work.

After that, I added time.sleep(2) between sending the S packet and the A packet. After looping for a few tries, I got the flag.
