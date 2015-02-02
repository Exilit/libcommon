#include <stdio.h>
 #include <openssl/ec.h>

#include "crypto.h"
#include "misc.h"

#include "check_common.h"
#include "check_crypto.h"


START_TEST (check_ec_signatures)
{
	EC_KEY *key;

	unsigned char *rdata, *sigdata;
	size_t dlens[] = { 16, 128, 1024, 65535 };
	size_t i, j, rsize, siglen, last_min = 1;
	int res;

	res = crypto_init();
	ck_assert_msg(!res, "Crypto initialization routine failed.\n");

	key = generate_ec_keypair(0);
	ck_assert_msg((key != NULL), "EC signature/verification check failed: could not generate key pair.\n");

	for (i = 0; i < (sizeof(dlens) / sizeof(dlens[0])); i++) {

		for (j = 0; j < N_SIGNATURE_TIER_TESTS; j++) {
			rdata = gen_random_data(last_min, dlens[i], &rsize);
			ck_assert_msg((rdata != NULL), "EC signature/verification check failed: could not generate random data.\n");
	
			sigdata = ec_sign_data(rdata, rsize, key, &siglen);
			ck_assert_msg((sigdata != NULL), "EC signature/verification check failed: could not sign data.\n");
			ck_assert_msg((siglen > 0), "EC signature/verification check failed: signature result had bad length.\n");

			res = verify_ec_signature(rdata, rsize, sigdata, siglen, key);
			ck_assert_msg((res == 1), "EC signature/verification check failed: signature verification failed (%d).\n", res);

			free(sigdata);
			free(rdata);
		}

		last_min = dlens[i];
	}

	free_ec_key(key);

	fprintf(stderr, "EC signature/verification check completed.\n");
}
END_TEST


START_TEST (check_ec_sha_signatures)
{
	EC_KEY *key;

	unsigned char *rdata, *sigdata;
	size_t dlens[] = { 16, 128, 1024, 65535 };
	size_t i, j, k, rsize, siglen, last_min = 1;
	unsigned int shabits;
	int res;

	res = crypto_init();
	ck_assert_msg(!res, "Crypto initialization routine failed.\n");

	key = generate_ec_keypair(0);
	ck_assert_msg((key != NULL), "EC SHA signature/verification check failed: could not generate key pair.\n");

	for (i = 0; i < (sizeof(dlens) / sizeof(dlens[0])); i++) {

		for (j = 0; j < N_SIGNATURE_TIER_TESTS; j++) {

			for (k = 0; k < 2; k++) {

				if (!k) {
					shabits = 160;
				} else if (k == 1) {
					shabits = 256;
				} else {
					shabits = 512;
				}

				rdata = gen_random_data(last_min, dlens[i], &rsize);
				ck_assert_msg((rdata != NULL), "EC SHA signature/verification check failed: could not generate random data.\n");
				sigdata = ec_sign_sha_data(rdata, rsize, shabits, key, &siglen);
				ck_assert_msg((sigdata != NULL), "EC SHA signature/verification check failed: could not sign data.\n");
				ck_assert_msg((siglen > 0), "EC SHA signature/verification check failed: signature result had bad length.\n");

				res = verify_ec_sha_signature(rdata, rsize, shabits, sigdata, siglen, key);
				ck_assert_msg((res == 1), "EC SHA signature/verification check failed: signature verification failed (%d).\n", res);

				free(sigdata);
				free(rdata);
			}

			last_min = dlens[i];
		}

	}


	free_ec_key(key);

	fprintf(stderr, "EC SHA signature/verification check completed.\n");
}
END_TEST


START_TEST (load_ec_key_file)
{
	const EC_GROUP *group;
	EC_KEY *result;
	char filename[256];
	size_t i;

	for (i = 0; i < 5; i++) {
		snprintf(filename, sizeof(filename), "ec-key-%zu-priv.pem", i+1);
		result = load_ec_privkey(filename, &group);
		ck_assert_msg(result != NULL, "load_ec_privkey failed for %s", filename);
		free_ec_key(result);

		snprintf(filename, sizeof(filename), "ec-key-%zu-pub.pem", i+1);
		result = load_ec_privkey(filename, &group);
		ck_assert_msg(result != NULL, "load_ec_privkey failed for %s", filename);
		free_ec_key(result);
	}

	fprintf(stderr, "EC key load from file check completed.\n");
}
END_TEST


START_TEST (check_ec_serialization)
{
	EC_KEY *pair, *pair2;
	unsigned char *sbuf, *sbuf2;
	int res;
	size_t i, ssize, ssize2;

	res = crypto_init();
	ck_assert_msg(!res, "Crypto initialization routine failed.\n");

	for (i = 0; i < N_SERIALIZATION_TESTS; i++) {
		pair = generate_ec_keypair(0);
		ck_assert_msg((pair != NULL), "EC serialization check failed: could not generate key pair.\n");

		sbuf = serialize_ec_pubkey(pair, &ssize);
		ck_assert_msg((sbuf != NULL), "EC serialization check failed: pubkey serialization error.\n");

		pair2 = deserialize_ec_pubkey(sbuf, ssize, 0);
		ck_assert_msg((pair2 != NULL), "EC serialization check failed: pubkey deserialization error.\n");

		sbuf2 = serialize_ec_pubkey(pair, &ssize2);
		ck_assert_msg((sbuf2 != NULL), "EC serialization check failed: pubkey serialization error [2].\n");

		ck_assert_msg((ssize == ssize2), "EC serialization check failed: serialized pubkeys had different serialized lengths {%u vs %u}\n", ssize, ssize2);

		res = memcmp(sbuf, sbuf2, ssize);
		ck_assert_msg(!res, "EC serialization check failed: serialized pubkeys had different data.\n");

		free(sbuf);
		free(sbuf2);
		free_ec_key(pair);
		free_ec_key(pair2);
	}

	fprintf(stderr, "EC serialization check completed.\n");
}
END_TEST


START_TEST (check_ed25519_signatures)
{
	ED25519_KEY *key;
	ed25519_signature sigbuf;
	unsigned char *rdata;
	size_t dlens[] = { 16, 128, 1024, 65535 };
	size_t i, j, rsize, last_min = 1;
	int res;

	res = crypto_init();
	ck_assert_msg(!res, "Crypto initialization routine failed.\n");

	key = generate_ed25519_keypair();
	ck_assert_msg((key != NULL), "ed25519 signature/verification check failed: could not generate key pair.\n");

	for (i = 0; i < (sizeof(dlens) / sizeof(dlens[0])); i++) {

		for (j = 0; j < N_SIGNATURE_TIER_TESTS; j++) {
			rdata = gen_random_data(last_min, dlens[i], &rsize);
			memset(sigbuf, 0, sizeof(sigbuf));
			ck_assert_msg((rdata != NULL), "ed25519 signature/verification check failed: could not generate random data.\n");
			ed25519_sign_data(rdata, rsize, key, sigbuf);

			res = ed25519_verify_sig(rdata, rsize, key, sigbuf);
			ck_assert_msg((res == 1), "ed25519 signature/verification check failed: signature verification failed (%d).\n", res);

			free(rdata);
		}

		last_min = dlens[i];
	}

	free_ed25519_key(key);

	fprintf(stderr, "ed25519 signature/verification check completed.\n");
}
END_TEST


Suite * suite_check_crypto(void) {

	Suite *s;
	TCase *tc;

	s = suite_create("crypto");
	testcase(s, tc, "EC Serialization/Deserialization", check_ec_serialization);
	testcase(s, tc, "EC Key Load From File", load_ec_key_file);
	testcase(s, tc, "EC Signing/Verification", check_ec_signatures);
	testcase(s, tc, "EC SHA Signing/Verification", check_ec_sha_signatures);
	testcase(s, tc, "ed25519 Signing/Verification", check_ed25519_signatures);

	return s;
}
