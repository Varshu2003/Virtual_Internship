#include <stdio.h>
#include <string.h>

// Need improvements to check for valid MAC address
void print_mac(const char *label, const char *mac) {
    printf("%s: %s\n", label, mac);
}


// 11n
void print_ht_capabilities() {
    printf("Tag: HT Capabilities (802.11n D1.10)\n");
    printf("Tag Number: 45\nTag length: 26\nHT Capabilities Info: 0x09ad\n");
    printf("A-MPDU Parameters: 0x17\nRx Supported MCS Set: MCS Set\n");
}

//11ac
void print_vht_capabilities() {
    printf("Tag: VHT Capabilities (802.11ac)\n");
    printf("Tag Number: 191\nTag length: 12\nVHT Capabilities Info: 0xf8b69b1\n");
    printf("VHT Supported MCS Set\n");
}

//11ax
void print_he_capabilities() {
    printf("Ext Tag: HE Capabilities (802.11ax)\n");
    printf("Ext Tag Number: 35\nExt Tag length: 35\n");
    printf("HE MAC Capabilities: 0x100012080005\nHE PHY Capabilities Information\n");
    printf("Supported HE-MCS and NSS Set\n");
}

int main() {
    char frame_type[20];
    char mac_address[20];
    char protocol[10];

    printf("1. Enter the mgmt frame type (Beacon / ProbeReq): ");
    scanf("%s", frame_type);

    printf("2. Enter the MAC address to be sent (RA): ");
    scanf("%s", mac_address);

    printf("3. Enter the 802.11 protocol (11n / 11ac / 11ax): ");
    scanf("%s", protocol);


    printf("\n----- Simulated 802.11 %s Frame -----\n", frame_type);
    printf("Frame: 492 bytes captured\n");
    printf("Radiotap Header, Length 56\n");
    printf("IEEE 802.11 %s frame, Flags: ....C\n", frame_type);
    printf("IEEE 802.11 Wireless Management\n");
    printf("Fixed parameters (12 bytes)\n");
    printf("Timestamp: 1234567890\n");
    printf("Beacon Interval: 0.102400 [Seconds]\n");
    printf("Capabilities Information: 0x1111\n");


    printf("Tagged parameters:\n");
    printf("Tag: SSID parameter set: \"test-network\"\n");
    printf("Tag Number: 0, Length: 13, SSID: test-network\n");
    printf("Tag: Supported Rates 6(B), Tag Number: 1, Length: 1\n");


    print_mac("Receiver Address (RA)", mac_address);
    print_mac("Transmitter Address (TA)", "aa:aa:aa:aa:aa:aa");


    if (strcmp(protocol, "11n") == 0) {
        print_ht_capabilities();
    } else if (strcmp(protocol, "11ac") == 0) {
        print_ht_capabilities();
        print_vht_capabilities();
    } else if (strcmp(protocol, "11ax") == 0) {
        print_ht_capabilities();
        print_vht_capabilities();
        print_he_capabilities();
    } else {
        printf("Unknown protocol.\n");
    }

    printf("-------------------------------------\n");

    return 0;
}
