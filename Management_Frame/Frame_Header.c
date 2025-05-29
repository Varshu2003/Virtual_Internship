#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint16_t frame_control;
    uint16_t duration;
    uint8_t receiver_address[6];
    uint8_t destination_address[6];
    uint8_t transmitter_address[6];
    uint8_t source_address[6];
    uint8_t bss_id[6];
    uint16_t seq_ctrl;
    uint32_t fcs;
} ieee80211_frame;

void set_mac(uint8_t *mac, const char *mac_str) {
    sscanf(mac_str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
}

void print_bit_flags(const char *label, uint8_t byte) {
    printf("%s: 0x%02x\n", label, byte);
    printf(".... .%d.. = DS status: Not leaving DS or network is operating in AD-HOC mode (To DS: 0 From DS: 0) (0x0)\n", (byte >> 3) & 1);
    printf(".... ..%d. = More Fragments: This is the last fragment\n", (byte >> 2) & 1);
    printf(".... ...%d = Retry: Frame is not being retransmitted\n", (byte >> 1) & 1);
    printf("...%d .... = PWR MGT: STA will stay up\n", (byte >> 4) & 1);
    printf("..%d. .... = More Data: No data buffered\n", (byte >> 5) & 1);
    printf(".%d.. .... = Protected flag: Data is not protected\n", (byte >> 6) & 1);
    printf("%d... .... = +HTC/Order flag: Not strictly ordered\n", (byte >> 7) & 1);
}

void print_mac_analysis(const char *label, uint8_t *mac) {
    printf("%s: %02x:%02x:%02x:%02x:%02x:%02x\n", label, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    printf(".... .%d.. .... .... = LG bit: %s\n", (mac[0] >> 1) & 1,
        ((mac[0] >> 1) & 1) ? "Locally administered address (this is NOT the factory default)" :
                              "Globally unique address (factory default)");
    printf(".... ..%d. .... .... = IG bit: %s\n", mac[0] & 1,
        (mac[0] & 1) ? "Group address (multicast/broadcast)" :
                      "Individual address (unicast)");
}

void hexdump(void *ptr, int buflen) {
    unsigned char *buf = (unsigned char *)ptr;
    for (int i = 0; i < buflen; i++) {
        printf("%02x ", buf[i]);
        if ((i % 16) == 15) printf("\n");
    }
    printf("\n");
}

int main() {
    ieee80211_frame frame;
    memset(&frame, 0, sizeof(frame));

    char type_input[32], recv_mac_input[32];
    uint8_t subtype;

    printf("Enter frame type (Beacon, Probe_Request, Probe_Response): ");
    scanf("%s", type_input);

    if (strcmp(type_input, "Beacon") == 0) {
        subtype = 8;
        set_mac(frame.receiver_address, "ff:ff:ff:ff:ff:ff");
    } else if (strcmp(type_input, "Probe_Request") == 0) {
        subtype = 4;
        printf("Enter receiver MAC address (format: xx:xx:xx:xx:xx:xx): ");
        scanf("%s", recv_mac_input);
        set_mac(frame.receiver_address, recv_mac_input);
    } else if (strcmp(type_input, "Probe_Response") == 0) {
        subtype = 5;
        set_mac(frame.receiver_address, "12:34:56:78:9a:bc");
    } else {
        printf("Invalid frame type.\n");
        return 1;
    }

    frame.frame_control = (0 << 0) | (0 << 2) | (subtype << 4);
    frame.duration = 0x0000;

    set_mac(frame.destination_address, "ff:ff:ff:ff:ff:ff");
    set_mac(frame.transmitter_address, "c4:f7:d5:4b:d3:83");
    set_mac(frame.source_address, "c4:f7:d5:4b:d3:83");
    set_mac(frame.bss_id, "c4:f7:d5:4b:d3:83");

    frame.seq_ctrl = (2523 << 4);  // fragment = 0, sequence = 2523
    frame.fcs = 0x5ceef133;

    printf("\nIEEE 802.11 %s frame, Flags:.......C",type_input);
    printf("\nType/Subtype: %s frame (0x%04x)\n", type_input, frame.frame_control);
    printf("Frame Control Field: 0x%04x\n", frame.frame_control);
    printf(".... ..00 = Version: 0\n");
    printf(".... 00.. = Type: Management frame (0)\n");
    printf(".... ..%04x = Subtype: %d\n", subtype, subtype);
    print_bit_flags("Flags", 0x00);
    printf("%04x = Duration: %d microseconds\n", frame.duration, frame.duration);

    print_mac_analysis("Receiver address", frame.receiver_address);
    print_mac_analysis("Destination address", frame.destination_address);
    print_mac_analysis("Transmitter address", frame.transmitter_address);
    print_mac_analysis("Source address", frame.source_address);
    print_mac_analysis("BSS Id", frame.bss_id);

    printf("0000 .... = Fragment number: %d\n", frame.seq_ctrl & 0xF);
    printf("1001 1101 1111 = Sequence number: %d\n", frame.seq_ctrl >> 4);
    printf("Frame check sequence: 0x%08x [unverified]\n", frame.fcs);
    printf("[FCS Status: Unverified]\n");
    printf("[WLAN Flags: ........]\n");

    printf("\nHexdump of frame header:\n");
    hexdump(&frame, sizeof(frame));

    return 0;
}
