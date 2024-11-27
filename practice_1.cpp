#include<iostream>
#include<gmp.h>

// 计算 (base^exponent)%mod
void power(mpz_t result, mpz_t base, mpz_t exponent, mpz_t mod) {
    mpz_powm(result, base, exponent, mod);
}

// 判断一个数是否为素数
bool is_prime(mpz_t n) {
    return mpz_probab_prime_p(n, 10) > 0;
}

// 计算模反元素，用于生成私钥指数 d
bool mod_inverse(mpz_t result, mpz_t a, mpz_t mod) {
    return mpz_invert(result, a, mod) != 0;
}

// RSA加密
void rsa_encrypt(mpz_t c, mpz_t d, mpz_t p, mpz_t q, mpz_t e, mpz_t m) {
    if (!is_prime(p) || !is_prime(q)) {
        std::cout << "错误：p 和 q 必须是素数。" << std::endl;
        return;
    }

    mpz_t n, phi, p_minus_1, q_minus_1;
    mpz_inits(n, phi, p_minus_1, q_minus_1, NULL);

    // 计算 n = p * q
    mpz_mul(n, p, q);

    // 计算 φ(n) = (p-1) * (q-1)
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_mul(phi, p_minus_1, q_minus_1);

    // 验证 e 是否与 φ(n) 互素，并计算私钥指数 d
    if (!mod_inverse(d, e, phi)) {
        std::cout << "错误：e 必须与 (p-1)*(q-1) 互素。" << std::endl;
        return;
    }

    // 计算密文 c = m^e mod n
    power(c, m, e, n);

    mpz_clears(n, phi, p_minus_1, q_minus_1, NULL);
}

// RSA解密
void rsa_decrypt(mpz_t m, mpz_t e, mpz_t p, mpz_t q, mpz_t d, mpz_t c) {
    if (!is_prime(p) || !is_prime(q)) {
        std::cout << "错误：p 和 q 必须是素数。" << std::endl;
        return;
    }

    mpz_t n;
    mpz_init(n);

    // 计算 n = p * q
    mpz_mul(n, p, q);

    // 计算明文 m = c^d mod n
    power(m, c, d, n);

    // 计算公钥指数 e
    mpz_t phi, p_minus_1, q_minus_1;
    mpz_inits(phi, p_minus_1, q_minus_1, NULL);

    // 计算 φ(n) = (p-1) * (q-1)
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_mul(phi, p_minus_1, q_minus_1);

    // 计算 e
    mod_inverse(e, d, phi);

    mpz_clears(n, phi, p_minus_1, q_minus_1, NULL);
}

int main() {
    mpz_t p, q, e, m, d, c, decrypted_m;
    mpz_inits(p, q, e, m, d, c, decrypted_m, NULL);
    int choice;

    std::cout << "请选择操作：" << std::endl;
    std::cout << "1. 加密" << std::endl;
    std::cout << "2. 解密" << std::endl;
    std::cin >> choice;

    if (choice == 1) {
        gmp_printf("请输入第一个素数 p: ");
        gmp_scanf("%Zd", p);
        gmp_printf("请输入第二个素数 q: ");
        gmp_scanf("%Zd", q);
        gmp_printf("请输入公钥指数 e: ");
        gmp_scanf("%Zd", e);
        gmp_printf("请输入需要加密的明文 m: ");
        gmp_scanf("%Zd", m);

        rsa_encrypt(c, d, p, q, e, m);
        gmp_printf("加密后的密文为: %Zd\n", c);
        gmp_printf("私钥指数 d 为: %Zd\n", d);
    } 
    else if (choice == 2) {
        gmp_printf("请输入第一个素数 p: ");
        gmp_scanf("%Zd", p);
        gmp_printf("请输入第二个素数 q: ");
        gmp_scanf("%Zd", q);
        gmp_printf("请输入私钥指数 d: ");
        gmp_scanf("%Zd", d);
        gmp_printf("请输入需要解密的密文 c: ");
        gmp_scanf("%Zd", c);

        rsa_decrypt(decrypted_m, e, p, q, d, c);
        gmp_printf("解密后的明文为: %Zd\n", decrypted_m);
        gmp_printf("公钥指数 e 为: %Zd\n", e);
    } 
    else {
        std::cout << "无效选择，程序退出。" << std::endl;
    }

    mpz_clears(p, q, e, m, d, c, decrypted_m, NULL);
    return 0;
}