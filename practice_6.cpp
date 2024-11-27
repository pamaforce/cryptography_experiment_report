#include <iostream>
#include <gmp.h>

// 使用 Shamir 窍门计算 g^a * h^b (mod p) 并统计乘法次数
void compute_and_count(mpz_t g, mpz_t h, mpz_t a, mpz_t b, mpz_t p, int& m, int& n) {
    mpz_t result, temp;
    mpz_inits(result, temp, NULL);

    // 计算 g^a mod p 使用 Shamir 的窍门
    mpz_powm(result, g, a, p);
    m += mpz_popcount(a);  // 统计 g^a 需要的乘法次数
    
    // 计算 h^b mod p 使用 Shamir 的窍门
    mpz_powm(temp, h, b, p);
    m += mpz_popcount(b);  // 统计 h^b 需要的乘法次数
    
    // 计算 g^a * h^b mod p
    mpz_mul(result, result, temp);
    mpz_mod(result, result, p);
    m++;  // 计算乘法

    // 打印 Shamir 的窍门结果
    gmp_printf("使用 Shamir 的窍门计算 g^a * h^b mod p = %Zd\n", result);

    // 使用扩展 Shamir 的窍门计算 g^a * h^b mod p
    mpz_t g_pow_a, h_pow_b;
    mpz_inits(g_pow_a, h_pow_b, NULL);
    
    // 预计算 g^a 和 h^b
    mpz_powm(g_pow_a, g, a, p);
    mpz_powm(h_pow_b, h, b, p);
    n += mpz_popcount(a) + mpz_popcount(b);  // 统计预计算的乘法次数

    // 计算 g^a * h^b mod p
    mpz_mul(result, g_pow_a, h_pow_b);
    mpz_mod(result, result, p);
    n++;  // 计算乘法

    // 打印扩展 Shamir 的窍门结果
    gmp_printf("使用扩展 Shamir 的窍门计算 g^a * h^b mod p = %Zd\n", result);

    mpz_clears(result, temp, g_pow_a, h_pow_b, NULL);
}

int main() {
    mpz_t g, h, a, b, p;
    mpz_inits(g, h, a, b, p, NULL);

    // 提示用户输入
    std::cout << "请输入底数 g：";
    gmp_scanf("%Zd", g);
    std::cout << "请输入底数 h：";
    gmp_scanf("%Zd", h);
    std::cout << "请输入指数 a：";
    gmp_scanf("%Zd", a);
    std::cout << "请输入指数 b：";
    gmp_scanf("%Zd", b);
    std::cout << "请输入模数 p：";
    gmp_scanf("%Zd", p);

    // 检查 p 是否符合要求
    if (mpz_cmp_ui(p, 0) <= 0 || mpz_cmp_ui(p, 1ULL << 32) > 0) {
        std::cerr << "模数 p 必须在 1 到 2^32 范围内！" << std::endl;
        mpz_clears(g, h, a, b, p, NULL);
        return 1;
    }

    int m = 0, n = 0;
    // 调用函数计算并统计乘法次数
    compute_and_count(g, h, a, b, p, m, n);

    // 输出乘法次数
    std::cout << "使用 Shamir 的窍门需要 " << m << " 次乘法\n";
    std::cout << "使用扩展 Shamir 的窍门需要 " << n << " 次乘法\n";

    mpz_clears(g, h, a, b, p, NULL);
    return 0;
}