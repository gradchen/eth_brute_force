all: eth_address_idx eth_brute_force

eth_brute_force:
	gcc -o eth_brute_force eth_brute_force.c keccak256.c -lsecp256k1

eth_address_idx:
	gcc -o eth_address_idx eth_address_idx.c

clean:
	rm -f eth_brute_force eth_address_idx result_*.txt

