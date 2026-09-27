#pragma once
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>

class HashUtils
{
public:
  static std::string sha256(const std::string &input)
  {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    return toHex(hash, SHA256_DIGEST_LENGTH);
  }

  // Salted PBKDF2-HMAC-SHA256, stored as "pbkdf2_sha256$<iterations>$<salt_hex>$<hash_hex>"
  static std::string hashPassword(const std::string &password)
  {
    unsigned char salt[SALT_LENGTH];
    if (RAND_bytes(salt, SALT_LENGTH) != 1)
    {
      throw std::runtime_error("Failed to generate password salt");
    }

    std::string saltHex = toHex(salt, SALT_LENGTH);
    return std::string(PBKDF2_PREFIX) + "$" + std::to_string(PBKDF2_ITERATIONS) + "$" + saltHex + "$" +
           pbkdf2(password, saltHex, PBKDF2_ITERATIONS);
  }

  static bool verifyPassword(const std::string &password, const std::string &stored)
  {
    if (isLegacyHash(stored))
    {
      return constantTimeEquals(sha256(password), stored);
    }

    // Expected: prefix$iterations$salt$hash
    size_t p1 = stored.find('$');
    size_t p2 = p1 == std::string::npos ? p1 : stored.find('$', p1 + 1);
    size_t p3 = p2 == std::string::npos ? p2 : stored.find('$', p2 + 1);
    if (p3 == std::string::npos || stored.substr(0, p1) != PBKDF2_PREFIX)
    {
      return false;
    }

    int iterations = 0;
    try
    {
      iterations = std::stoi(stored.substr(p1 + 1, p2 - p1 - 1));
    }
    catch (const std::exception &)
    {
      return false;
    }
    if (iterations <= 0)
    {
      return false;
    }

    std::string saltHex = stored.substr(p2 + 1, p3 - p2 - 1);
    std::string expected = stored.substr(p3 + 1);
    return constantTimeEquals(pbkdf2(password, saltHex, iterations), expected);
  }

  // True for hashes created before PBKDF2 (unsalted SHA-256) or with fewer iterations
  static bool needsRehash(const std::string &stored)
  {
    return stored.rfind(std::string(PBKDF2_PREFIX) + "$" + std::to_string(PBKDF2_ITERATIONS) + "$", 0) != 0;
  }

private:
  static constexpr const char *PBKDF2_PREFIX = "pbkdf2_sha256";
  // OWASP recommendation for PBKDF2-HMAC-SHA256
  static constexpr int PBKDF2_ITERATIONS = 600000;
  static constexpr int SALT_LENGTH = 16;

  static std::string pbkdf2(const std::string &password, const std::string &salt, int iterations)
  {
    unsigned char derived[SHA256_DIGEST_LENGTH];
    if (PKCS5_PBKDF2_HMAC(password.c_str(), static_cast<int>(password.size()),
                          reinterpret_cast<const unsigned char *>(salt.c_str()), static_cast<int>(salt.size()),
                          iterations, EVP_sha256(), SHA256_DIGEST_LENGTH, derived) != 1)
    {
      throw std::runtime_error("Failed to hash password");
    }
    return toHex(derived, SHA256_DIGEST_LENGTH);
  }

  static bool isLegacyHash(const std::string &stored)
  {
    return stored.size() == SHA256_DIGEST_LENGTH * 2 &&
           stored.find_first_not_of("0123456789abcdef") == std::string::npos;
  }

  static bool constantTimeEquals(const std::string &a, const std::string &b)
  {
    return a.size() == b.size() && CRYPTO_memcmp(a.data(), b.data(), a.size()) == 0;
  }

  static std::string toHex(const unsigned char *data, size_t length)
  {
    std::stringstream ss;
    for (size_t i = 0; i < length; i++)
    {
      ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    return ss.str();
  }
};
