#include "dcrypto.h"
#include "error.h"


int crypto_init(void) {
	PUBLIC_FUNC_IMPL(crypto_init);
}

void crypto_shutdown(void) {
	PUBLIC_FUNC_IMPL(crypto_shutdown);
}

int verify_ec_signature(const unsigned char *hash, size_t hlen, const unsigned char *sig, size_t slen, EC_KEY *key) {
	PUBLIC_FUNC_IMPL(verify_ec_signature, hash, hlen, sig, slen, key);
}

int verify_ec_sha_signature(const unsigned char *data, size_t dlen, unsigned int shabits, const unsigned char *sig, size_t slen, EC_KEY *key) {
	PUBLIC_FUNC_IMPL(verify_ec_sha_signature, data, dlen, shabits, sig, slen, key);
}

unsigned char * ec_sign_data(const unsigned char *hash, size_t hlen, EC_KEY *key, size_t *siglen) {
	PUBLIC_FUNC_IMPL(ec_sign_data, hash, hlen, key, siglen);
}

unsigned char * ec_sign_sha_data(const unsigned char *data, size_t dlen, unsigned int shabits, EC_KEY *key, size_t *siglen) {
	PUBLIC_FUNC_IMPL(ec_sign_sha_data, data, dlen, shabits, key, siglen);
}

unsigned char * serialize_ec_pubkey(EC_KEY *key, size_t *outsize) {
	PUBLIC_FUNC_IMPL(serialize_ec_pubkey, key, outsize);
}

EC_KEY * deserialize_ec_pubkey(const unsigned char *buf, size_t blen, int signing) {
	PUBLIC_FUNC_IMPL(deserialize_ec_pubkey, buf, blen, signing);
}

unsigned char * serialize_ec_privkey(EC_KEY *key, size_t *outsize) {
	PUBLIC_FUNC_IMPL(serialize_ec_privkey, key, outsize);
}

EC_KEY * deserialize_ec_privkey(const unsigned char *buf, size_t blen, int signing) {
	PUBLIC_FUNC_IMPL(deserialize_ec_privkey, buf, blen, signing);
}

EC_KEY * load_ec_privkey(const char *filename, const EC_GROUP **gptr) {
	PUBLIC_FUNC_IMPL(load_ec_privkey, filename, gptr);
}

EC_KEY * generate_ec_keypair(int signing) {
	PUBLIC_FUNC_IMPL(generate_ec_keypair, signing);
}

void free_ec_key(EC_KEY *key) {
	PUBLIC_FUNC_IMPL(free_ec_key, key);
}

ED25519_KEY * generate_ed25519_keypair(void) {
	PUBLIC_FUNC_IMPL(generate_ed25519_keypair);
}

int ed25519_sign_data(const unsigned char *data, size_t dlen, ED25519_KEY *key, ed25519_signature sigbuf) {
	PUBLIC_FUNC_IMPL(ed25519_sign_data, data, dlen, key, sigbuf);
}

int ed25519_verify_sig(const unsigned char *data, size_t dlen, ED25519_KEY *key, ed25519_signature sigbuf) {
	PUBLIC_FUNC_IMPL(ed25519_verify_sig, data, dlen, key, sigbuf);
}

void free_ed25519_key(ED25519_KEY *key) {
	PUBLIC_FUNC_IMPL(free_ed25519_key, key);
}

ED25519_KEY * load_ed25519_privkey(const char *filename) {
	PUBLIC_FUNC_IMPL(load_ed25519_privkey, filename);
}

void * ecies_env_derivation(const void *input, size_t ilen, void *output, size_t *olen) {
	PUBLIC_FUNC_IMPL(ecies_env_derivation, input, ilen, output, olen);
}

int compute_ec_ephemeral_aes256_key(EC_KEY *key, EC_KEY *ephemeral, unsigned char *keybuf) {
	PUBLIC_FUNC_IMPL(compute_ec_ephemeral_aes256_key, key, ephemeral, keybuf);
}

int get_random_bytes(void *buf, size_t len) {
	PUBLIC_FUNC_IMPL(get_random_bytes, buf, len);
}

int encrypt_aes_256(unsigned char *outbuf, const unsigned char *data, size_t dlen, const unsigned char *key, const unsigned char *iv) {
	PUBLIC_FUNC_IMPL(encrypt_aes_256, outbuf, data, dlen, key, iv);
}

int decrypt_aes_256(unsigned char *outbuf, const unsigned char *data, size_t dlen, const unsigned char *key, const unsigned char *iv) {
	PUBLIC_FUNC_IMPL(decrypt_aes_256, outbuf, data, dlen, key, iv);
}
