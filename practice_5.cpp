#include <gmp.h>
#include <vector>
#include <iostream>

// 计算 wNAF 表示
std::vector<int> wNAF(mpz_t k, int w) {
    std::vector<int> wNAF_rep; // 用于存储 wNAF 表示
    mpz_t zero, temp, two_pow_w, two_pow_w_minus_1;
    mpz_inits(zero, temp, two_pow_w, two_pow_w_minus_1, NULL);
    
    // 计算 2^w 和 2^(w-1)
    mpz_ui_pow_ui(two_pow_w, 2, w);
    mpz_ui_pow_ui(two_pow_w_minus_1, 2, w - 1);

    while (mpz_cmp(k, zero) > 0) { // 当 k > 0
        if (mpz_odd_p(k)) { // 如果 k 是奇数
            // 计算 z_i = k mod 2^w
            mpz_mod(temp, k, two_pow_w);
            if (mpz_cmp(temp, two_pow_w_minus_1) >= 0) { // 如果 z_i >= 2^(w-1)
                mpz_sub(temp, temp, two_pow_w);          // z_i = z_i - 2^w
            }
            wNAF_rep.push_back(mpz_get_si(temp));        // 存储 z_i
            // 更新 k = k - z_i
            mpz_sub(k, k, temp);
        } else {
            wNAF_rep.push_back(0); // 如果 k 是偶数，z_i = 0
        }
        // k 右移 1 位
        mpz_tdiv_q_2exp(k, k, 1);
    }

    mpz_clears(zero, temp, two_pow_w, two_pow_w_minus_1, NULL);
    return wNAF_rep;
}

int main() {
    mpz_t k;
    mpz_init(k);

    int w;
    std::cout << "请输入一个正整数 k：";
    gmp_scanf("%Zd", k);

    std::cout << "请输入窗口大小 w：";
    std::cin >> w;

    // 计算 wNAF 表示
    std::vector<int> wNAF_rep = wNAF(k, w);

    // 输出结果
    std::cout << "整数 k 的窗口宽度为 " << w << " 的 wNAF 表示为：" << std::endl;
    for (int i = wNAF_rep.size() - 1; i >= 0; --i) {
        std::cout << wNAF_rep[i] << " ";
    }
    std::cout << std::endl;

    mpz_clear(k);
    return 0;
}