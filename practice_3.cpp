#include <gmp.h>
#include <iostream>
#include <cmath>

// 求最大素因子函数
void largestPrimeFactor(mpz_t n, mpz_t result) {
    mpz_t i, max;
    mpz_inits(i, max, NULL);

    // 处理因数 2
    while (mpz_divisible_ui_p(n, 2)) {
        mpz_set_ui(max, 2);
        mpz_divexact_ui(n, n, 2);
    }

    // 检查奇数因子
    for (mpz_set_ui(i, 3); mpz_cmp(i, n) <= 0; mpz_add_ui(i, i, 2)) {
        while (mpz_divisible_p(n, i)) {
            mpz_set(max, i);
            mpz_divexact(n, n, i);
        }
    }

    mpz_set(result, max);
    mpz_clears(i, max, NULL);
}

// 主函数
int main() {
    mpz_t p, s, r, t, temp;
    mpz_inits(p, s, r, t, temp, NULL);
    gmp_randstate_t state;
    gmp_randinit_mt(state);

    unsigned int l;
    std::cout << "请输入强素数的比特长度 (0 < l <= 32)：";
    std::cin >> l;

    if (l <= 0 || l > 32) {
        std::cerr << "错误：比特长度必须在 0 < l <= 32 范围内！" << std::endl;
        return 1;
    }

    // 随机生成指定比特长度的数
    mpz_urandomb(p, state, l);

    // 确保是奇数
    if (mpz_even_p(p)) {
        mpz_add_ui(p, p, 1);
    }

    while (true) {
        // 检查 p 是否为素数
        if (mpz_probab_prime_p(p, 25)) {
            // 计算 p+1 的最大素因子
            mpz_add_ui(temp, p, 1);
            largestPrimeFactor(temp, s);

            // 计算 p-1 的最大素因子
            mpz_sub_ui(temp, p, 1);
            largestPrimeFactor(temp, r);

            // 计算 r-1 的最大素因子
            mpz_sub_ui(temp, r, 1);
            largestPrimeFactor(temp, t);

            // 检查 s、r、t 的比特长度是否符合要求
            unsigned int half = std::ceil(l / 2.0);
            if (mpz_sizeinbase(s, 2) >= half - 4 && mpz_sizeinbase(s, 2) <= half + 4 &&
                mpz_sizeinbase(r, 2) >= half - 4 && mpz_sizeinbase(r, 2) <= half + 4 &&
                mpz_sizeinbase(t, 2) >= half - 4 && mpz_sizeinbase(t, 2) <= half + 4) {
                break;
            }
        }

        // 若不满足强素数条件，尝试下一个奇数
        mpz_add_ui(p, p, 2);
    }

    // 输出结果
    std::cout << "强素数 p：";
    mpz_out_str(stdout, 10, p);
    std::cout << "\np+1 的最大素因子 s：";
    mpz_out_str(stdout, 10, s);
    std::cout << "\np-1 的最大素因子 r：";
    mpz_out_str(stdout, 10, r);
    std::cout << "\nr-1 的最大素因子 t：";
    mpz_out_str(stdout, 10, t);
    std::cout << std::endl;

    mpz_clears(p, s, r, t, temp, NULL);
    gmp_randclear(state);

    return 0;
}