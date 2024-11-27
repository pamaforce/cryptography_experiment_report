#include <iostream>
#include <gmp.h>

// 检查两个数是否互素
bool is_coprime(mpz_t a, mpz_t b) {
    mpz_t gcd;
    mpz_init(gcd);
    mpz_gcd(gcd, a, b);
    bool result = mpz_cmp_ui(gcd, 1) == 0;
    mpz_clear(gcd);
    return result;
}

// 计算模反元素
void modInverse(mpz_t a, mpz_t m, mpz_t inv) {
    mpz_t g, x, y;
    mpz_inits(g, x, y, NULL);
    mpz_gcdext(g, x, y, a, m);
    if (mpz_cmp_ui(g, 1) != 0) {
        std::cerr << "错误：无法计算模反元素，输入的值不是互素的。" << std::endl;
        exit(EXIT_FAILURE);
    }
    mpz_mod(inv, x, m); // 确保结果非负
    mpz_clears(g, x, y, NULL);
}

int main() {
    mpz_t e, n[3], c[3], m, N, prod, inv, temp;
    mpz_inits(e, m, N, prod, inv, temp, NULL);
    for (int i = 0; i < 3; i++) {
        mpz_inits(n[i], c[i], NULL);
    }

    std::cout << "请输入小加密指数 e (例如 3): ";
    mpz_inp_str(e, stdin, 10);

    std::cout << "请输入三个互素的模数 n1, n2, n3 (每行一个):" << std::endl;
    for (int i = 0; i < 3; i++) {
        mpz_inp_str(n[i], stdin, 10);
    }

    // 检查模数是否两两互素
    if (!is_coprime(n[0], n[1]) || !is_coprime(n[0], n[2]) || !is_coprime(n[1], n[2])) {
        std::cerr << "错误：模数 n1, n2, n3 必须两两互素。" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "请输入对应的密文 c1, c2, c3 (每行一个):" << std::endl;
    for (int i = 0; i < 3; i++) {
        mpz_inp_str(c[i], stdin, 10);
    }

    // 计算所有 n 的乘积 N
    mpz_set_ui(N, 1);
    for (int i = 0; i < 3; i++) {
        mpz_mul(N, N, n[i]);
    }

    // 使用中国剩余定理恢复合并的密文
    mpz_set_ui(m, 0);
    for (int i = 0; i < 3; i++) {
        // 计算 N / n[i]
        mpz_div(prod, N, n[i]);

        // 计算 (N / n[i]) 的模反元素
        modInverse(prod, n[i], inv);

        // temp = c[i] * (N / n[i]) * inv
        mpz_mul(temp, c[i], prod);
        mpz_mul(temp, temp, inv);

        // 累加到 m
        mpz_add(m, m, temp);
        mpz_mod(m, m, N);
    }

    // 计算 m 的 e 次根，恢复原始明文
    if (!mpz_root(m, m, mpz_get_ui(e))) {
        std::cerr << "错误：无法计算 e 次方根，可能输入的参数不正确。" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "恢复的明文消息 m 为: ";
    mpz_out_str(stdout, 10, m);
    std::cout << std::endl;

    mpz_clears(e, m, N, prod, inv, temp, NULL);
    for (int i = 0; i < 3; i++) {
        mpz_clears(n[i], c[i], NULL);
    }

    return 0;
}