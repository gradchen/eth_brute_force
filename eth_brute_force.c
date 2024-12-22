#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <secp256k1.h>
#include "keccak256.h"

// Function to generate public key from private key
int generate_public_key(const unsigned char *private_key, unsigned char *public_key) {
    secp256k1_context *ctx;
    secp256k1_pubkey pubkey;

    // Initialize the secp256k1 context
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, private_key)) {
        printf("Error generating public key\n");
        secp256k1_context_destroy(ctx);
        return -1;
    }

    size_t pubkey_len = 65;  // Compressed public key length (33 for compressed, 65 for uncompressed)
    secp256k1_ec_pubkey_serialize(ctx, public_key, &pubkey_len, &pubkey, SECP256K1_EC_UNCOMPRESSED);

    secp256k1_context_destroy(ctx);
    return 0;
}

void ethers_keccak256(const uint8_t *data, uint16_t length, uint8_t *result) {

    SHA3_CTX context;
    keccak_init(&context);
    keccak_update(&context, (const unsigned char*)data, (size_t)length);
    keccak_final(&context, (unsigned char*)result);

    // Clear out the contents of what we hashed (in case it was secret)
    memset((char*)&context, 0, sizeof(SHA3_CTX));
}

// Function to generate Ethereum address from public key
void generate_ethereum_address(const unsigned char *public_key, unsigned char *address) {
    unsigned char hash[32];

    // Perform Keccak-256 hash on the public key
    ethers_keccak256(public_key + 1, 64, hash);  // Skip the first byte (compressed prefix)

    // The address is the last 20 bytes of the hash
    memcpy(address, hash + 12, 20);
}

int brute_force_try_one(int show, int process_idx) {
    unsigned char private_key[32];

    FILE *fp = fopen("/dev/urandom", "rb");
    fread(private_key, 1, 32, fp);
    fclose(fp);

//#define ETH_KEYS_DEBUG 1
#if ETH_KEYS_DEBUG
    printf("Private Key (Hex): ");
    for (int i = 0; i < 32; i++) printf("%02x", private_key[i]);
    printf("\n");
#endif

    unsigned char public_key[65];
    unsigned char address[20];  // Ethereum address (20 bytes)

    // Generate public key from private key
    if (generate_public_key(private_key, public_key) != 0) {
        return 1;
    }
#if ETH_KEYS_DEBUG
    printf("Uncompressed Public Key (Hex): ");
    for (size_t i = 0; i < 65; i++) printf("%02x", public_key[i]);
    printf("\n");
#endif


    // Generate Ethereum address from public key
    generate_ethereum_address(public_key, address);

#if ETH_KEYS_DEBUG
    // Print the Ethereum address
    printf("Ethereum Address: 0x");
    for (int i = 0; i < 20; i++) {
        printf("%02x", address[i]);
    }
    printf("\n");
#endif

    char str_address[43];
    snprintf(str_address, sizeof(str_address),
        "0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        address[0], address[1], address[2], address[3], address[4],
        address[5], address[6], address[7], address[8], address[9],
        address[10], address[11], address[12], address[13], address[14],
        address[15], address[16], address[17], address[18], address[19]);
//#define ETH_TEST_OUTPUT_FILE 1
#if ETH_TEST_OUTPUT_FILE
    snprintf(str_address, sizeof(str_address), "%s", "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2");
#endif

    char filepath[128];
    snprintf(filepath, sizeof(filepath), "/tmp/eth/0x%c%c%c/%c%c%c/%s",
        str_address[2], str_address[3], str_address[4],
        str_address[5], str_address[6], str_address[7],
        str_address);

    if (show)
        printf("filepath: %s\n", filepath);

    if (access(filepath, F_OK) == 0) {
        char output_file[128];
        snprintf(output_file, sizeof(output_file), "./result_%d.txt", process_idx);
        FILE *file = fopen(output_file, "a"); // Open file in append mode
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        fprintf(file, "Private Key (Hex): ");
        for (int i = 0; i < 32; i++) fprintf(file, "%02x", private_key[i]);
        fprintf(file, "\n");

        fclose(file); // Close the file
    }

    return 0;
}

int brute_force_loop(int process_idx) {
    long long int counter = 0;
    while (1) {
        counter++;
        if ((counter % 100) == 0)
            printf("[chidl %d] Tried: %lld\n", process_idx, counter);

        brute_force_try_one(0, process_idx);
    }

    return 0;
}


pid_t child_pids[512];
int g_process_num = 4;

// Signal handler for SIGINT
void handle_sigint(int sig) {
    printf("\nParent received SIGINT. Terminating child processes...\n");
    for (int i = 0; i < g_process_num; i++) {
        if (child_pids[i] > 0) {
            printf("Terminating child process %d\n", child_pids[i]);
            kill(child_pids[i], SIGTERM);
        }
    }
    exit(0);
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("Usage: %s <num_process>\n", argv[0]);
        return -1;
    }

    g_process_num = atoi(argv[1]);
    // Create child processes
    for (int i = 0; i < g_process_num; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            brute_force_loop(i);
        } else {
            // Parent process
            child_pids[i] = pid;
        }
    }

    sleep(1);
    printf("Parent process %d waiting for Ctrl+C... to terminate ETH brute-force\n", getpid());

    // Set up SIGINT signal handler
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    // Parent process waits indefinitely
    while (1) {
        pause(); // Wait for signals
    }

    return 0;
}
